const express = require('express');
const router = express.Router();
const auth = require('./auth');
const accountModel = require('../models/account_model');
const transactionModel = require('../models/transaction_model');
const cardModel = require('../models/card_model');
const moment = require('moment-timezone');

const lockWithdraw = {};

const unlockWithdraw = (idcard) => {
    setTimeout(() => {
        delete lockWithdraw[idcard];
    }, 100);
};

// Pyyntöjen käsittely.
router.post('/', auth, (req, res) => {
    const idcard = req.user.idcard;
    const amount = parseFloat(req.body.amount);

    const allowedAmounts = [20, 40, 50, 60, 80, 100, 200, 500];

    if (!allowedAmounts.includes(amount)) {
        return res.status(400).json({ error: 'Invalid withdrawal. Allowed values are 20, 40, 50, 60, 80, 100, 200, and 500.' });
    }

    if (!idcard || isNaN(amount) || amount <= 0) {
        return res.status(400).json({ error: 'Invalid withdrawal.' });
    }

    if (lockWithdraw[idcard]) {
        return res.status(429).json({ error: 'Wait a moment before another withdrawal.' });
    }

    lockWithdraw[idcard] = true;
    checkAccountType(idcard, amount, res);
});

// Vaihe 1: Tarkistetaan, onko tili debit vai credit.
function checkAccountType(idcard, amount, res) {
    accountModel.getAccountDetails(idcard, (err, result) => {
        if (err) return handleError(res, idcard, 'Server error.', 500);
        if (!result.length) return handleError(res, idcard, 'Account not found.', 404);

        const { balance, type_credit, credit_limit } = result[0];

        if (type_credit === 0) {
            // Debit-tili. Tarkistetaan, riittääkö saldo.
            if (balance >= amount) {
                updateBalance(idcard, balance, amount, res);
            } else {
                return handleError(res, idcard, 'Insuffecient funds.', 400);
            }
        } else if (type_credit === 1) {
            // Credit-tili. Saldo + luottoraja.
            const totalFunds = parseFloat(balance) + parseFloat(credit_limit);
            if (totalFunds >= amount) {
                updateBalance(idcard, balance, amount, res);
            } else {
                return handleError(res, idcard, 'Credit limit exceeded.', 400);
            }
        } else {
            return handleError(res, idcard, 'Unknown account type.', 500);
        }
    });
}

// Vaihe 2 Päivitetään saldo.
function updateBalance(idcard, balance, amount, res) {
    accountModel.updateBalance(idcard, balance - amount, (err) => {
        if (err) return handleError(res, idcard, 'Server error.', 500);
        getIdAccount(idcard, balance, amount, res);
    });
}

// Vaihe 3: Hae tilin ID.
function getIdAccount(idcard, balance, amount, res) {
    cardModel.getIdAccount(idcard, (err, result) => {
        if (err || !result.length) return handleError(res, idcard, 'Server error.', 500);

        const idaccount = result[0].idaccount;
        addTransaction(idaccount, balance, amount, idcard, res);
    });
}

// Vaihe 4: Lisää tilitapahtuma.
function addTransaction(idaccount, balance, amount, idcard, res) {
    const transactionTime = moment().tz("Europe/Helsinki").format("YYYY-MM-DD HH:mm:ss");
    const newBalance = balance - amount;

    transactionModel.addTransaction(transactionTime, amount, idaccount, (err) => {
        unlockWithdraw(idcard);
        if (err) return res.status(500).json({ error: 'Server error.' });

        res.status(200).json({ newBalance });
    });
}

// Virheenkäsittely.
function handleError(res, idcard, message, status) {
    unlockWithdraw(idcard);
    res.status(status).json({ error: message });
}

module.exports = router;
