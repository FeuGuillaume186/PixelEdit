/*######################################################################################################
###
###   Projet Name : PixelEdit
###   Projet Vers : Beta 0.1.3c
###   Auteur      : Guillaume M.
###   Studio      : Pixello Dev.
###   Debut       : 01/08/2020 01:30
###   Fin         : 01/09/2020 05:18
###   Mis a jour  : 16/09/2020 15:35
###   Mis a jour* : 02/01/2021 19:29
###   Mis a jour* : 22/04/2021 21:36
###   Mis a jour* : 27/04/2021 01:00
###   Description : Editeur d'icon en format .pix en console win/linux
###	  Mails		  : guillaumemulenga@gmail.com
###
###	  (c) 2020 Pixello Developper.
###
#######################################################################################################*/
#include "Editeur.hpp"

using namespace std;

// prototype
void Opening_Initializing();
AppTheme Theme_Defaut();

// Global Variables
static Editeur pixelEdit(Theme_Defaut());

int main()
{
    int v(11);
    String console = "mode con lines=" + to_string(WINDOW_HAUT) + " cols=" + to_string(WINDOW_LARG);
    system(console.c_str());

    Opening_Initializing();

    while(pixelEdit.Get_editorState())
    {
        if(v != -11)
            pixelEdit.GUI_Affichage_Interface_Actuelle();
        v = pixelEdit.GUI_Gestion_Menu_Bar();
    }

    return 0;
}

void Opening_Initializing()
{
    // Variables
    ifstream verifyFile;
    String desc[4], answ[3];
    BoxTheme boxColor;
    ElementTheme answStyle;
    BoxTools *boiteDeDialogue(0);
    Pix logo;

    // initialisation window
    SetConsoleTitleA("Pixel Edit 0.1.4");
    clrscr();

    //Make_Directory(DATA_DIR_NAME);
    logo.Open_Pix_File(LOGO_PE_PATH);

    /******************************************/
    boxColor.default_background = BLANC_BRILLANT;
    boxColor.default_foreground = NOIR;
    boxColor.title_background = BLEU_GRIS;
    boxColor.title_foreground = BLANC_BRILLANT;
    boxColor.content_background = BLANC;
    boxColor.content_foreground = BLEU_GRIS;
    boxColor.content_foreground_inactive = GRIS;
    boxColor.shadow_color = NOIR;

    answStyle.normal_background = BLANC;
    answStyle.normal_foreground = NOIR;
    answStyle.select_background = BLEU_GRIS;
    answStyle.select_foreground = BLANC_BRILLANT;
    /******************************************/

    boiteDeDialogue = new BoxTools(boxColor, answStyle);

    // Affichage
    Change_Console_Color(_BLANC_BRILLANT, _GRIS);
    logo.Apercu({(short)((WINDOW_LARG - logo.Get_dimensions().largeur) / 2), (short)(((WINDOW_HAUT - logo.Get_dimensions().hauteur)/2)-1)}, false);

    // verification
    verifyFile.open(LOGO_PE_PATH);
    if(verifyFile.fail())
    {
        desc[0] = "Impossible de demarer PixelEdit";
        desc[1] = "car le fichier suivant est manquant";
        desc[2] = LOGO_PE_NAME;
        answ[0] = "Quitter";
        boiteDeDialogue->DialogueBox_Style_0("COMPOSANT MAQUANT !", desc, answ, 3, 1, false);
        for(int i(0); i < 10; ++i) chargementPoint(1, VERT_CLAIR, BLANC_BRILLANT, 89, DBOX_POS_X+3, DBOX_POS_Y+(DBOX_HAUT-2));
        exit(0);
    }
    Sleep(1000);

    //for(int i(0); i < 20; ++i) chargementPoint(1, BLANC, GRIS, 130, (WINDOW_LARG - 1) / 2, WINDOW_HAUT - 3);
    chargementBarre1(53, 15, 8, 12, 150, (WINDOW_LARG - 53)/2, WINDOW_HAUT-15);
    Sleep(1000);

    next_page(BLANC, NOIR, 60);

    // free disk space
    delete boiteDeDialogue;
    boiteDeDialogue = 0;
}

AppTheme Theme_Defaut_back()
{
    AppTheme app;

    // Barre de menu (Menu Principal)
    app.MenuBar.normal_background = GRIS;
    app.MenuBar.normal_foreground = BLANC_BRILLANT;
    app.MenuBar.select_background = BLANC_BRILLANT;
    app.MenuBar.select_foreground = NOIR;

    // Barre de statut
    app.StatBar.normal_background = BLANC_BRILLANT;
    app.StatBar.normal_foreground = GRIS;

    // Les menus deroulants
    app.ListeDeroulant.normal_background = BLANC_BRILLANT;
    app.ListeDeroulant.normal_foreground = GRIS;
    app.ListeDeroulant.select_background = CYAN;
    app.ListeDeroulant.select_foreground = NOIR;

    // Boite de Dialogue
    app.BoiteDeDialogue.default_background = BLANC_BRILLANT;
    app.BoiteDeDialogue.default_foreground = NOIR;
    app.BoiteDeDialogue.title_background = BLEU;
    app.BoiteDeDialogue.title_foreground = BLANC_BRILLANT;
    app.BoiteDeDialogue.content_background = BLANC;
    app.BoiteDeDialogue.content_foreground = VERT_CLAIR;
    app.BoiteDeDialogue.content_foreground_inactive = GRIS;
    app.BoiteDeDialogue.shadow_color = NOIR;

    app.BoiteDeDialogueRep.normal_background = BLANC;
    app.BoiteDeDialogueRep.normal_foreground = GRIS;
    app.BoiteDeDialogueRep.select_background = BLEU;
    app.BoiteDeDialogueRep.select_foreground = BLANC_BRILLANT;

    // Boite normal (avec des trucs dedant)
    app.BoiteNormal.default_background = GRIS;
    app.BoiteNormal.default_foreground = BLANC_BRILLANT;
    app.BoiteNormal.title_background = BLEU_CLAIR;
    app.BoiteNormal.title_foreground = BLANC_BRILLANT;
    app.BoiteNormal.content_background = BLANC;
    app.BoiteNormal.content_foreground = BLEU;
    app.BoiteNormal.content_foreground_inactive = GRIS;
    app.BoiteNormal.shadow_color = NOIR;

    app.BoiteNormalRep.normal_background = BLANC_BRILLANT;
    app.BoiteNormalRep.normal_foreground = GRIS;
    app.BoiteNormalRep.select_background = BLEU_CLAIR;
    app.BoiteNormalRep.select_foreground = BLANC_BRILLANT;

    // Barre de chargement
    app.BarreDeChargement = BLEU_CLAIR;

    // Les ListeDeroulant des outils
    app.ToolBar.default_background = GRIS;
    app.ToolBar.default_foreground = BLANC_BRILLANT;
    app.ToolBar.element_background = BLANC_BRILLANT;
    app.ToolBar.element_foreground = GRIS;

    // Les outils
    app.Tools_EtatNormal.normal_background = GRIS;
    app.Tools_EtatNormal.normal_foreground = BLANC_BRILLANT;
    app.Tools_EtatNormal.select_background = GRIS;
    app.Tools_EtatNormal.select_foreground = BLEU_CLAIR;
    app.Tools_EtatActif.select_background = GRIS;
    app.Tools_EtatActif.select_foreground = ROUGE;

    return app;
}

AppTheme Theme_Defaut()
{
    AppTheme app;

    // Barre de menu (Menu Principal)
    app.MenuBar.normal_background = BLANC_BRILLANT;
    app.MenuBar.normal_foreground = NOIR;
    app.MenuBar.select_background = BLEU_CLAIR;
    app.MenuBar.select_foreground = BLANC_BRILLANT;

    // Barre de statut
    app.StatBar.normal_background = BLANC_BRILLANT;
    app.StatBar.normal_foreground = NOIR;

    // Les menus deroulants
    app.ListeDeroulant.normal_background = BLANC_BRILLANT;
    app.ListeDeroulant.normal_foreground = GRIS;
    app.ListeDeroulant.select_background = JAUNE_CLAIR;
    app.ListeDeroulant.select_foreground = NOIR;

    // Boite de Dialogue
    app.BoiteDeDialogue.default_background = BLANC_BRILLANT;
    app.BoiteDeDialogue.default_foreground = GRIS;
    app.BoiteDeDialogue.title_background = JAUNE_CLAIR;
    app.BoiteDeDialogue.title_foreground = NOIR;
    app.BoiteDeDialogue.content_background = BLANC;
    app.BoiteDeDialogue.content_foreground = JAUNE;
    app.BoiteDeDialogue.content_foreground_inactive = GRIS;
    app.BoiteDeDialogue.shadow_color = GRIS;

    app.BoiteDeDialogueRep.normal_background = BLANC;
    app.BoiteDeDialogueRep.normal_foreground = GRIS;
    app.BoiteDeDialogueRep.select_background = JAUNE_CLAIR;
    app.BoiteDeDialogueRep.select_foreground = NOIR;

    // Boite normal (avec des trucs dedant)
    app.BoiteNormal.default_background = BLANC_BRILLANT;
    app.BoiteNormal.default_foreground = GRIS;
    app.BoiteNormal.title_background = JAUNE_CLAIR;
    app.BoiteNormal.title_foreground = NOIR;
    app.BoiteNormal.content_background = BLANC;
    app.BoiteNormal.content_foreground = JAUNE;
    app.BoiteNormal.content_foreground_inactive = GRIS;
    app.BoiteNormal.shadow_color = GRIS;

    app.BoiteNormalRep.normal_background = BLANC;
    app.BoiteNormalRep.normal_foreground = GRIS;
    app.BoiteNormalRep.select_background =JAUNE_CLAIR;
    app.BoiteNormalRep.select_foreground = NOIR;

    // Barre de chargement
    app.BarreDeChargement = VERT_CLAIR;

    // Les ListeDeroulant des outils
    app.ToolBar.default_background = BLANC_BRILLANT;
    app.ToolBar.default_foreground = GRIS;
    app.ToolBar.element_background = GRIS;
    app.ToolBar.element_foreground = BLANC_BRILLANT;

    // Les outils
    app.Tools_EtatNormal.normal_background = BLANC_BRILLANT;
    app.Tools_EtatNormal.normal_foreground = NOIR;
    app.Tools_EtatNormal.select_background = JAUNE_CLAIR;
    app.Tools_EtatNormal.select_foreground = NOIR;
    app.Tools_EtatActif.select_background = JAUNE;
    app.Tools_EtatActif.select_foreground = BLANC_BRILLANT;

    return app;
}
