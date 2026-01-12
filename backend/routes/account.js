const auth = require('./auth');
const express = require('express');
const jwt = require('jsonwebtoken'); // Lisätty JWT
const account = require('../models/account_model');
const router = express.Router();
const user = require('../models/account_model');

router.get('/', auth, (req, res) => {
  const id = req.user.idcard;

  user.getByCardId(id, function (err, dbResult) {
    if (err) {
      res.status(500).json({ message: 'Internal server error', err });
    } else {
      const dbObject = dbResult[0];
      if (dbObject === undefined) {
        res.status(404).json({ message: 'No account by that id' });
      } else {
        res.status(200).json({ message: 'Fetched account', account: dbObject });
      }
    }
  });
});

router.post('/', function (req, res) {
  account.add(req.body, function (err, dbResult) {
    if (err) {
      res.status(500).json({ message: 'Internal server error', err });
    } else {
      res.json(req.body);
    }
  });
});

router.delete('/:id', function (req, res) {
  account.delete(req.params.id, function (err, dbResult) {
    if (err) {
      res.status(500).json({ message: 'Internal server error', err });
    } else {
      res.json(dbResult);
    }
  });
});

router.put('/:id', function (req, res) {
  account.update(req.params.id, req.body, function (err, dbResult) {
    if (err) {
      res.status(500).json({ message: 'Internal server error', err });
    } else {
      res.json(dbResult);
    }
  });
});

module.exports = router;