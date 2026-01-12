const db = require('../database');

const user={
  getByCardId: function(id, callback) {
    return db.query('SELECT * FROM user WHERE iduser IN (SELECT iduser FROM account INNER JOIN card ON account.idaccount = card.idaccount WHERE idcard=?)',
      [id], callback);
  },
  add: function(userObj, callback) {
    return db.query('INSERT INTO user (fname,lname) VALUES(?,?)',
      [userObj.fname, userObj.lname], callback);
  },
  update: function(id, userObj, callback) {
    return db.query('UPDATE user SET fname=?,lname=? WHERE iduser=?',
      [userObj.fname, userObj.lname, id], callback);
  },
  delete: function(id, callback) {
    return db.query('DELETE FROM user WHERE iduser=?',
      [id], callback);
    }
}

module.exports = user;