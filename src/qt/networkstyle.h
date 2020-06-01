// Copyright (c) 2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_QT_NETWORKSTYLE_H
#define BITCOIN_QT_NETWORKSTYLE_H

#include <QIcon>
#include <QPixmap>
#include <QString>

/* Coin network-specific GUI style information */
class NetworkStyle
{
public:
    /** Get style associated with provided BIP70 network id, or 0 if not known */
    static  NetworkStyle *instantiate( QString &networkId);

     QString &getAppName()  { return appName; }
     QIcon &getAppIcon()  { return appIcon; }
     QIcon &getTrayAndWindowIcon()  { return trayAndWindowIcon; }
     QString &getTitleAddText()  { return titleAddText; }

private:
    NetworkStyle( QString &appName,  int iconColorHueShift,  int iconColorSaturationReduction,  char *titleAddText);

    QString appName;
    QIcon appIcon;
    QIcon trayAndWindowIcon;
    QString titleAddText;
};

#endif // BITCOIN_QT_NETWORKSTYLE_H
