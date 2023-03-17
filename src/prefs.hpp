#ifndef SETTINGS_HPP_INCLUDED
#define SETTINGS_HPP_INCLUDED

#define WINDOW_WIDTH 350
#define WINDOW_HEIGHT 250
#define DISCWINDOW_WIDTH 600
#define DISCWINDOW_HEIGHT 500
#define WINDOW_NAME "ipms 1.0"
#define DISCUSSION_WIDTH 300
#define DISCUSSION_HEIGHT 250
#define PORT 6529

enum IDs {
    NouveauContact = 1,
    ImportContact = 2,
    ListeContacts = 4,
    Preferences = 5,
    ClientDesc = 6,
    Socket = 7,
    Server = 8,
    ButStart = 9,
    SrvSock = 10,
    SrvDesc = 11,
    ListBox = 12,
    FormatBold = 13,
    FormatItalic = 14,
    FormatUnderline = 15,
    FormatAlignLeft = 16,
    FormatAlignCentre = 16,
    FormatAlignRight = 17,
    FormatIndentLess = 18,
    FormatIndentMore = 19,
    FormatFont = 20,
    ButEnvoyer = 21,
    SettingMaj = 22,
    SettingCmd = 23,
    SettingsValider = 24,
    SettingSplashScreen = 25,
    ButReload = 26,
    EditProfils = 27
};

#define RGE   "\x1B[31m"
#define VRT   "\x1B[32m"
#define JNE   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define BLC   "\x1B[37m"
#define RESET "\x1B[0m"

#endif // SETTINGS_HPP_INCLUDED