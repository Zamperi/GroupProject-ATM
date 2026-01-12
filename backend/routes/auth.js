const jwt = require('jsonwebtoken');

module.exports = function (req, res, next) {
    //Hae header
    const authHeader = req.header('Authorization');
    if (!authHeader) return res.status(401).json({ error: 'Token puuttuu.' });

    // Tarkista, että header on olemassa ja sisältää Bearer-tokenin
    if (!authHeader || !authHeader.startsWith('Bearer ')) {
        return res.status(401).json({ error: 'Token puuttuu tai on virheellinen.' });
    }

    //Erota Bearer ja token
    const token = authHeader.split(' ')[1];
    if (!token) return res.status(401).json({ error: 'Token puuttuu.' });

    try {
        const verified = jwt.verify(token, process.env.JWT_SECRET); // Validointi
        req.user = verified; // Tallentaa käyttäjätiedot
        next(); // Siirry seuraavaan middlewareen
    } catch (err) {
        res.status(400).json({ error: 'Virheellinen token.' });
    }
};
