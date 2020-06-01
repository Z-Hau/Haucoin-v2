// Copyright (c) 2011-2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_QT_ADDRESSTABLEMODEL_H
#define BITCOIN_QT_ADDRESSTABLEMODEL_H

#include <QAbstractTableModel>
#include <QStringList>

enum OutputType : int;

class AddressTablePriv;
class WalletModel;

class CWallet;

/**
   Qt model of the address book in the core. This allows views to access and modify the address book.
 */
class AddressTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit AddressTableModel(CWallet *wallet, WalletModel *parent = 0);
    ~AddressTableModel();

    enum ColumnIndex {
        Label = 0,   /**< User specified label */
        Address = 1  /**< Bitcoin address */
    };

    enum RoleIndex {
        TypeRole = Qt::UserRole /**< Type of address (#Send or #Receive) */
    };

    /** Return status of edit/insert operation */
    enum EditStatus {
        OK,                     /**< Everything ok */
        NO_CHANGES,             /**< No changes were made during edit operation */
        INVALID_ADDRESS,        /**< Unparseable address */
        DUPLICATE_ADDRESS,      /**< Address already in address book */
        WALLET_UNLOCK_FAILURE,  /**< Wallet could not be unlocked to create new receiving address */
        KEY_GENERATION_FAILURE  /**< Generating a new public key for a receiving address failed */
    };

    static  QString Send;      /**< Specifies send address */
    static  QString Receive;   /**< Specifies receive address */

    /** @name Methods overridden from QAbstractTableModel
        @{*/
    int rowCount( QModelIndex &parent) ;
    int columnCount( QModelIndex &parent) ;
    QVariant data( QModelIndex &index, int role) ;
    bool setData( QModelIndex &index,  QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) ;
    QModelIndex index(int row, int column,  QModelIndex &parent) ;
    bool removeRows(int row, int count,  QModelIndex &parent = QModelIndex());
    Qt::ItemFlags flags( QModelIndex &index) ;
    /*@}*/

    /* Add an address to the model.
       Returns the added address on success, and an empty string otherwise.
     */
    QString addRow( QString &type,  QString &label,  QString &address,  OutputType address_type);

    /* Look up label for address in address book, if not found return empty string.
     */
    QString labelForAddress( QString &address) ;

    /* Look up row index of an address in the model.
       Return -1 if not found.
     */
    int lookupAddress( QString &address) ;

    EditStatus getEditStatus()  { return editStatus; }

private:
    WalletModel *walletModel;
    CWallet *wallet;
    AddressTablePriv *priv;
    QStringList columns;
    EditStatus editStatus;

    /** Notify listeners that data changed. */
    void emitDataChanged(int index);

public Q_SLOTS:
    /* Update address list from core.
     */
    void updateEntry( QString &address,  QString &label, bool isMine,  QString &purpose, int status);

    friend class AddressTablePriv;
};

#endif // BITCOIN_QT_ADDRESSTABLEMODEL_H
