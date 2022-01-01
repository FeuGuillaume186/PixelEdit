/*
*   Pix.cpp
*
*   Fichier source de la class Pix, cette class gere le mecanisme de creation,
*   ouverture et autres mecanisme liee au fichier de type pix.
*
*   (c) 2020 Pixello Developpers
*/
#include "../inc/Pix.hpp"

using namespace std;

Pix::Pix(): m_nom(""), m_repertoire(""), m_path(m_repertoire + m_nom), m_dimensions({0, 0}), m_taille(0)
{
    m_logFile = "PixFile";
}

Pix::Pix(String nomDuFichier, String repertoire, Dimensions dimensions): m_nom(nomDuFichier), m_repertoire(repertoire), m_dimensions(dimensions)
{
    //ctor
    m_logFile = "PixFile";

    // initialise le chemin d'acces complet du fichier
    m_path = m_repertoire + m_nom;
    ///!Setup_Directory(m_path);

    for(int i(0); i < m_dimensions.hauteur; ++i)
        m_pixels.push_back(vector<Pixel>(m_dimensions.largeur, {0, (short)i, BLANC_BRILLANT, NOIR, '\xB0'}));

    for(int i(0); i < m_dimensions.hauteur; ++i)
    {
        for(int j(0); j < m_dimensions.largeur; ++j)
            m_pixels[i][j].X = j;
    }

    this->Update_Taille();
}

Pix::Pix(const Pix& other): m_nom(other.m_nom), m_repertoire(other.m_repertoire),  m_path(other.m_path), m_dimensions(other.m_dimensions), m_lastTime(other.m_lastTime), m_lastDate(other.m_lastDate),
m_taille(other.m_taille), m_pixels(other.m_pixels), m_logFile(other.m_logFile)
{}

Pix::~Pix()
{
    Log_Out(m_logFile);
}

Pix& Pix::operator=(const Pix& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    m_path = rhs.m_path;
    m_nom = rhs.m_nom;
    m_repertoire = rhs.m_repertoire;
    m_dimensions = rhs.m_dimensions;
    m_lastTime = rhs.m_lastTime;
    m_lastDate = rhs.m_lastDate;
    m_taille = rhs.m_taille;
    m_pixels = rhs.m_pixels;
    return *this;
}

// name: Update_Taille
// desc: met a jour le poid du fichier
void Pix::Update_Taille()
{
    ifstream file(m_path, ios::binary);
    file.seekg(0, ios::end);
    m_taille = file.tellg();
    if(m_taille < 0) m_taille = 0;
}

// name: Update_Recent_Files
// desc: met a jour la list de fichiers recent
bool Pix::Update_Recent_Files()
{
    // Variables
    fstream files, files1;
    String path, filesPath(RECENT_FILE_PATH), var; // "pec: Pixel Edit Configuration Files"
    bool fileFound(false);
    int n(0);

    // ouverture du fichier qui contient la list de fichiers recent
    files.open(filesPath, ios::in | ios::binary);
    files1.open(filesPath, ios::in | ios::binary);
    if(files.fail())
    {
        files.open(filesPath, ios::out | ios::binary);
        if(files.fail()) return false;
        files.close();
        files.open(filesPath, ios::in | ios::binary);
    }

    // cout
    while (getline(files1, var))
        ++n;

    if(n >= MAX_RECENTFILES-2)
    {
        Write_In_Log(m_logFile, "La liste des fichiers recents est pleine veullez la vider pour pouvoir l'utiliser.");
        return false;
    }

    // Verifie si le fichier actuel est deja sur la list
    while(!fileFound && getline(files, path))
    {
        if(path == m_path) fileFound = true;
        else fileFound = false;
    }

    // Si il y est deja on l'ajoute plus
    if(fileFound) return false;

    // sinon on l'ajoute sur la liste
    files.close();
    files.open(filesPath, ios::out | ios::app | ios::binary);
    files << m_path << endl;
    return true;
}

// name: Save_Pix_File
// desc: Sauvegarde le fichier pix ouvert actuellement
bool Pix::Save_Pix_File()
{
    // Variables
    fstream file;

    // Initialisations
    m_lastTime.setTime();  // met a jour l'heure de la derniere sauvegarde
    m_lastDate.setDate();  // met a jour la date de la derniere sauvegarde
    this->Update_Taille(); // met a jour la taille du fichier

    // Verifie si le fichier existe
    file.open(m_path, ios::in | ios::binary);
    if(file.fail())
        return false;

    // Si il existe beuh on commence la sauvegarde
    file.close(); // ferme le fichier ouvert en mode lecture pour l'ouvrir en mode ecriture
    file.open(m_path, ios::out | ios::binary);
    // Sauvegarde des metadonnees
    file << m_nom << endl;
    file << m_repertoire << endl;
    file << m_path << endl;
    file << m_dimensions.hauteur << ' ';
    file << m_dimensions.largeur << ' ';
    file << m_lastTime << ' ';
    file << m_lastDate << ' ';
    file << m_taille << endl;

    // Sauvegarde du fichier en soit
    for(int i(0); i < m_dimensions.hauteur; ++i)
    {
        for(int j(0); j < m_dimensions.largeur; ++j)
            file << m_pixels[i][j].X << ' ' << m_pixels[i][j].Y << ' ' << m_pixels[i][j].f << ' ' << m_pixels[i][j].t << ' ' << m_pixels[i][j].C << ' ';
        file << endl;
    }

    Write_In_Log(m_logFile, "Save_Pix_File: Sauvegarde du fichier: \"" + m_path + "\"...succes.");
    return true;
}

// name: New_Pix_File
// desc: cree un nouveau fichier Pix
int Pix::New_Pix_File()
{
    // Variables et initialisations
    fstream file;

    // Verifie si le fichier existe deja ou pas
    file.open(m_path, ios::in | ios::binary);
    if(file.fail())
    {
        // s'il n'existe pas on le cree
        file.close();
        file.open(m_path, ios::out | ios::binary);
        // verifie si le fichier a ete cree
        if(file.fail())
        {
            // si non, c'est forcement parceque le dossier n'existe pas
            // alors on cree le dossier en question puis on cree le fichier
            Make_Directory(m_repertoire);
            file.open(m_path, ios::out | ios::binary);
            // si il ne cree toujours pas le fichier alors la je ne sais pas
            if(file.fail())
            {
                Write_In_Log(m_logFile, "New_Pix_File: Ne parvient pas a cree le fichier: \"" + m_path + "\" pour une raison inconnue.");
                return -1; // erreur
            }
        }

        // puis on le sauvegarde si tout va bien
        this->Save_Pix_File();

        Write_In_Log(m_logFile, "New_Pix_File: Creation du fichier: \"" + m_path + "\"...succes.");
        return 0; // tout se bien passer
    }

    Write_In_Log(m_logFile, "New_Pix_File: Le fichier: \"" + m_path + "\" existe deja.");
    return 1; // fichier existant
}

// name: Open_Pix_File
// desc: Ouvre un fichier Pix uniquement seulement un fichier Pix
bool Pix::Open_Pix_File(String path)
{
    // Variables et initialisation
    ifstream file;
    int tmp;

    // ouverture du fichier
    file.open(path, ios::binary);
    if(file.fail())
        return false; // fichier n'existe pas

    // s'il existe
    // Charge les metadonnees
    getline(file, m_nom);
    getline(file, m_repertoire);
    getline(file, m_path);
    file >> m_dimensions.hauteur >> m_dimensions.largeur >> m_lastTime >> m_lastDate >> m_taille;

    // si le fichier est trafiquer
    if((m_dimensions.hauteur > NEWFILE_MAX_SIZE_HAUT || m_dimensions.largeur > NEWFILE_MAX_SIZE_LARG) || (m_dimensions.hauteur < 1 || m_dimensions.largeur < 1))
        return false;

    // Charge les valeurs des pixels
    for(int i(0); i < m_dimensions.hauteur; ++i)
    {
        // allocation de l'espace pour stocker le contenu du fichier
        m_pixels.push_back(vector<Pixel>(m_dimensions.largeur, {0, 0, BLANC_BRILLANT, NOIR, '\xB0'}));

        for(int j(0); j < m_dimensions.largeur; ++j)
        {
            file >> m_pixels[i][j].X >> m_pixels[i][j].Y >> m_pixels[i][j].f >> m_pixels[i][j].t >> m_pixels[i][j].C;
            if(j == m_dimensions.largeur)
                file >> tmp;
        }
    }

    Write_In_Log(m_logFile, "Open_Pix_File: Ouverture du fichier: \"" + m_path + "\"...succes.");
    return true; // Tout se bien passer
}

// name: Show_Pix_File_In_Editor
// desc: affiche le fichier dans l'editeur
void Pix::Show_Pix_File_In_Editor(bool showShadow)
{
    for (int i(0); i < this->Get_dimensions().hauteur; ++i)
    {
        // shadow
        Change_Color(8, 8);
        if(showShadow)
            Print_To(10 + (((WINDOW_LARG - 43) - this->Get_dimensions().largeur) / 2) + 1, ((WINDOW_HAUT - this->Get_dimensions().hauteur) / 2) + i + 1, To_String(this->Get_dimensions().largeur));

        for(int j(0); j < this->Get_dimensions().largeur; ++j)
            this->Show_Pixel(j,i);
    }
}

// name: Show_Pixel
// desc: affiche une uniquement la valeur d'un pixel du fichier
void Pix::Show_Pixel(short x, short y)
{
    Change_Color(this->Get_pixels()[y][x].f, this->Get_pixels()[y][x].t);
    Print_To(10 + (((WINDOW_LARG - 43) - this->Get_dimensions().largeur) / 2) + x, ((WINDOW_HAUT - this->Get_dimensions().hauteur) / 2) + y, this->Get_pixels()[y][x].C);
}

// name: Apercu
// desc: done m'apercu du fichier a la position donnee en parametre
void Pix::Apercu(Coords pos, bool shadow)
{
    // file
    for (int i(0); i < this->Get_dimensions().hauteur; ++i)
    {
        // shadow
        if(shadow)
        {
            Change_Color(8, 8);
            Print_To(pos.X + 1, pos.Y + 1 + i , To_String(this->Get_dimensions().largeur));
        }

        for(int j(0); j < this->Get_dimensions().largeur; ++j)
        {
            Change_Color(this->Get_pixels()[i][j].f, this->Get_pixels()[i][j].t);
            Print_To(pos.X + j, pos.Y + i, this->Get_pixels()[i][j].C);
        }
    }
}
