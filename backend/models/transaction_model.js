const db = require('../database');
const moment = require('moment-timezone');
  
const transaction = {
    getTransactions: function(idaccount, callback) {
        db.query('SELECT idtransaction, idaccount, amount, time FROM transaction WHERE idaccount = ? ORDER BY time DESC LIMIT 10',
            [idaccount], 
            (err, results) => {
                if (err) return callback(err);
    
                // Muunnetaan aika Suomen aikavyöhykkeeseen
                results.forEach(tx => {
                    tx.time = moment(tx.time).tz('Europe/Helsinki').format('DD-MM-YYYY HH:mm:ss');
                });
                callback(null, results);
            }
        );
    },
    
    addTransaction: function(time, amount, idaccount, callback) {
        return db.query('INSERT INTO transaction (time, amount, idaccount) VALUES (?,?,?)',
        [time, amount, idaccount], callback);
    },

    // Nyt kaikki metodit ovat transaction-objektissa
    getById: function(id, callback) {
        return db.query('SELECT * FROM transaction WHERE idaccount IN (SELECT card.idaccount FROM account INNER JOIN card ON account.idaccount = card.idaccount WHERE idcard = ?)', [id], callback);
    },

    add: function(transaction, callback) {
        return db.query(
            'INSERT INTO transaction (idaccount, time, amount) VALUES (?, ?, ?)',
            [transaction.idaccount, transaction.time, transaction.amount],
            callback
        );
    },

    delete: function(id, callback) {
        return db.query('DELETE FROM transaction WHERE idtransaction = ?', [id], callback);
    },

    update: function(id, transaction, callback) {
        return db.query(
            'UPDATE transaction SET idaccount = ?, time = ?, amount = ? WHERE idtransaction = ?',
            [transaction.idaccount, transaction.time, transaction.amount, id],
            callback
        );
    }
};

module.exports = transaction;