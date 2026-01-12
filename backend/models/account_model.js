const db = require('../database');
const account = {


    getByCardId: function(id, callback) {
        return db.query('SELECT * FROM account WHERE idaccount IN (SELECT idaccount FROM card WHERE idcard=?)',
            [id], callback
        );
    },
    updateBalance: function(idcard, newBalance, callback) {
        return db.query('UPDATE account SET balance =? WHERE idaccount =(SELECT idaccount FROM card WHERE idcard=?)',
            [newBalance, idcard], callback);
    },

    getAccountDetails: function(idcard, callback) {
        return db.query(
            `SELECT account.balance, account.type_credit, account.credit_limit FROM account JOIN card ON card.idaccount = account.idaccount WHERE card.idcard = ?`, [idcard], callback
        );
    },

    getById: function(id, callback) {
        return db.query('select * from transaction where idaccount in (select idaccount from account inner join card on account.idaccount=card.idaccount where idcard=?)', [id], callback);
    },

    add: function(account, callback) {
        return db.query(
            'insert into account (balance, type_credit, credit_limit, iduser) values (?,?,?,?)',
            [account.balance, account.type_credit, account.credit_limit, account.iduser],
            callback
        );
    },

    delete: function(id, callback) {
        return db.query('delete from account where idaccount=?', [id], callback);
    },

    update: function(id, account, callback) {
        return db.query(
            'update account set balance=?, type_credit=?, credit_limit=?, iduser=? WHERE idaccount=?',
            [account.balance, account.type_credit, account.credit_limit, account.iduser, id],
            callback
        );
    }

  };
module.exports = account;