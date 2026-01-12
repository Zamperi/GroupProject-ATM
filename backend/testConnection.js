const db = require('./database');

db.query('SELECT 1 + 1 AS solution', function (err, results, fields) {
    if (err) {
        console.error('Tietokantayhteys epäonnistui:', err);
    } else {
        console.log('Tietokantayhteys onnistui, tulos:', results[0].solution);
    }
});
