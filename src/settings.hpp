#ifndef SETTINGS_HPP_INCLUDED
#define SETTINGS_HPP_INCLUDED

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 300
#define DISCWINDOW_WIDTH 600
#define DISCWINDOW_HEIGHT 500
#define WINDOW_NAME "ipms 1.0"
#define DISCUSSION_WIDTH 300
#define DISCUSSION_HEIGHT 250
#define PORT 6529
#define FILEPERMS 777

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
    ButEnvoyer = 21
};
#endif // SETTINGS_HPP_INCLUDED