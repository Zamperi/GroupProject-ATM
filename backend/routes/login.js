const express = require('express');
const router = express.Router();
const card = require('../models/card_model');
const jwt = require('jsonwebtoken');
const dotenv = require('dotenv');
const bcrypt = require('bcryptjs');

dotenv.config();

let failedLoginAttempts = {};
const lockLogin = {};

// Lukituksen vapauttaminen
const unlockLogin = (cardnum) => {
    delete lockLogin[cardnum];
};


//Reitin käsittely
router.post('/', (req, res) => {
    const cardnum = req.body.cardnum;
    const pin = req.body.pin;

    if (!cardnum || !pin) {
        return res.status(400).json({ error: 'Card number and PIN required.' });
    }

    if (lockLogin[cardnum]) {
        return res.status(429).json({ error: 'Please wait a moment before trying again.' });
    }

    if (failedLoginAttempts[cardnum] && Date.now() < failedLoginAttempts[cardnum]) {
        return res.status(429).json({ error: 'Please wait a moment before trying again.' });
    }

    lockLogin[cardnum] = true;
    checkLockedStatus(cardnum, pin, res);
});

// Vaihe 1: Tarkista, onko kortti lukittu
function checkLockedStatus(cardnum, pin, res) {
    card.checkLockedStatus(cardnum, (err, result) => {
        if (err) return handleError(res, cardnum, 'Database error.', 500);
        if (!result.length) return handleError(res, cardnum, 'Login failed.', 404);

        if (result[0].locked === 1) {
            return handleError(res, cardnum, 'Please contact customer services.', 403);
        }

        checkPin(cardnum, pin, res);
    });
}

// Vaihe 2: Tarkista PIN
function checkPin(cardnum, pin, res) {
    card.checkPin(cardnum, (err, result) => {
        if (err) return handleError(res, cardnum, 'Database error.', 500);
        if (!result.length) return handleError(res, cardnum, 'Login failed.', 404);

        const hashedPin = result[0].pin;
        bcrypt.compare(pin, hashedPin, (err, isMatch) => {
            if (err) return handleError(res, cardnum, 'Database error.', 500);

            if (!isMatch) {
                incrementAttempts(cardnum, res);
            } else {
                resetAttempts(cardnum, res);
            }
        });
    });
}

// Vaihe 3: Lisää epäonnistunut yritys ja lukitse tarvittaessa
function incrementAttempts(cardnum, res) {
    card.incrementAttempts(cardnum, (err) => {
        if (err) return handleError(res, cardnum, 'Database error.', 500);

        card.getAttempts(cardnum, (err, result) => {
            if (err) return handleError(res, cardnum, 'Database error.', 500);

            const attempts = result[0].attempts;
            if (attempts >= 3) {
                lockCard(cardnum, res);
            } else {
                failedLoginAttempts[cardnum] = Date.now() + 3000;
                return handleError(res, cardnum, 'Login failed. Wait 3 seconds before trying again.', 401);
            }
        });
    });
}

// Vaihe 4: Lukitse kortti (jos liian monta epäonnistunutta yritystä)
function lockCard(cardnum, res) {
    card.lockCard(cardnum, (err) => {
        if (err) return handleError(res, cardnum, 'Database error.', 500);
        return handleError(res, cardnum, 'Please contact customer services.', 403);
    });
}

// Vaihe 5: PIN oikein, nollaa yritykset ja luo token
function resetAttempts(cardnum, res) {
    card.resetAttempts(cardnum, (err) => {
        if (err) return handleError(res, cardnum, 'Database error.', 500);

        card.getIdCard(cardnum, (err, result) => {
            if (err) return handleError(res, cardnum, 'Database error.', 500);

            const idcard = result[0].idcard;
            const token = jwt.sign({ idcard: idcard }, process.env.JWT_SECRET, { expiresIn: '1800s' });
            unlockLogin(cardnum);
            return res.status(200).json({ message: token });
        });
    });
}

// Virheenkäsittely
function handleError(res, cardnum, message, status) {
    unlockLogin(cardnum);
    res.status(status).json({ error: message });
}

module.exports = router;
