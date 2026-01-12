const express = require('express');
const router = express.Router();
const transactionModel = require('../models/transaction_model');
const jwt = require('jsonwebtoken');
const auth = require('./auth');
const cardModel = require('../models/card_model');

router.get('/latestTransactions', auth, (req, res)=> {
    const idcard = req.user.idcard;

    cardModel.getIdAccount(idcard, (err, result) => {
        if (err) return res.status(500).json({ error: 'Tilin ID:n haku epäonnistui.' });

        if (result.length === 0) return res.status(404).json({ error: 'Tiliä ei löytynyt.' });

        const idaccount = result[0].idaccount;
        transactionModel.getTransactions(idaccount, (err, result)=>{
            if (err) return res.status(500).json({ error: 'Tilitapahtumien haku epäonnistui.' });

            res.status(200).json(result);

        }); 

    });
});

router.get('/', auth, (req, res) => {
  const idcard = req.user.idcard;
  transactionModel.getById(idcard, function(err, dbResult) {
      if (err) {
          return res.status(500).json({ error: 'Tilitapahtumien hakeminen epäonnistui.' });
      } else {
          res.status(200).json({
              message: 'Tilitapahtumat haettu onnistuneesti.',
              data: dbResult
          });
      }
  });
});

router.post('/', function(req, res) {
  transactionModel.add(req.body, function(err, dbResult) {
      if (err) {
          return res.status(500).json({ error: 'Tilitapahtuman lisääminen epäonnistui.' });
      } else {
          res.status(201).json({
              message: 'Tilitapahtuma lisätty onnistuneesti.',
              data: req.body
          });
      }
  });
});

router.delete('/:id', function(req, res) {
  transactionModel.delete(req.params.id, function(err, dbResult) {
      if (err) {
          return res.status(500).json({ error: 'Tilitapahtuman poistaminen epäonnistui.' });
      } else {
          res.status(200).json({
              message: 'Tilitapahtuma poistettu onnistuneesti.',
              data: dbResult
          });
      }
  });
});

router.put('/:id', function(req, res) {
  transactionModel.update(req.params.id, req.body, function(err, dbResult) {
      if (err) {
          return res.status(500).json({ error: 'Tilitapahtuman päivittäminen epäonnistui.' });
      } else {
          res.status(200).json({
              message: 'Tilitapahtuma päivitetty onnistuneesti.',
              data: dbResult
          });
      }
  });
});

module.exports = router;