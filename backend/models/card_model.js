const db = require('../database');
const bcrypt = require('bcryptjs');

const card = {
    getAll: function(callback) {
        return db.query('SELECT * FROM card', callback);
    },

    getById: function(cardnum, callback) {
        return db.query('SELECT * FROM card WHERE cardnum = ?', [cardnum], callback);
    },

    add: function(card_data, callback) {
        bcrypt.hash(card_data.pin, 10, function(err, hashed_pin) {
            if (err) return callback(err);
            
            return db.query(
                'INSERT INTO card (idcard, cardnum, pin, attempts, locked, idaccount ) VALUES (?, ?, ?, ?, ?, ?)',
                [ card_data.idcard, card_data.cardnum, hashed_pin,card_data.attempts, card_data.locked, card_data.idaccount],
                callback
            );
        });
    },

    update: function(idcard, card_data, callback) {
        bcrypt.hash(card_data.pin, 10, function(err, hashed_pin) {
            if (err) return callback(err);
            
            return db.query(
                'UPDATE card SET idaccount =?, cardnum=?, pin = ?, attempts =?, locked = ?, idaccount=?  WHERE idcard = ?',
                [card_data.idaccount, card_data.cardnum, hashed_pin, card_data.attempts, card_data.locked, card_data.idaccount, idcard],
                callback
            );
        });
    },

    delete: function(cardnum, callback) {
        return db.query('DELETE FROM card WHERE cardnum = ?', [cardnum], callback);
    },

    getIdCard: function(cardnum, callback) {
        return db.query('SELECT idcard FROM card WHERE cardnum = ?', [cardnum], callback)
    },
    
    getIdAccount: function(idcard, callback) {
        return db.query('SELECT idaccount FROM card WHERE idcard = ?', [idcard], callback);
    },    

    getIdAccount: function(idcard, callback) {
        return db.query('SELECT idaccount FROM card WHERE idcard = ?', [idcard], callback);
    },    

    checkPin: function(cardnum, callback) {
        return db.query('SELECT pin FROM card WHERE cardnum = ?', [cardnum], callback);
    },

    checkLockedStatus: function(cardnum, callback){
        return db.query('SELECT locked FROM card WHERE cardnum = ?', [cardnum], callback);
    },

    unlockCard: function(cardnum, callback){
        return db.query('UPDATE card SET locked = 0 WHERE cardnum = ?', [cardnum], callback);
    },

    lockCard: function(cardnum, callback){
        return db.query('UPDATE card SET locked = 1 WHERE cardnum = ?', [cardnum], callback);
    },

    getAttempts: function(cardnum, callback) {
        return db.query('SELECT attempts FROM card WHERE cardnum = ?',[cardnum], callback);
    },

    incrementAttempts: function (cardnum, callback) {
        return db.query('UPDATE card SET attempts = attempts + 1 WHERE cardnum=?', [cardnum], callback);
    },

    resetAttempts: function (cardnum, callback) {
        return db.query('UPDATE card SET attempts = 0 WHERE cardnum=?', [cardnum], callback);
    }
};

module.exports = card;
