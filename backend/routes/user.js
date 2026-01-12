const express = require('express');
const router = express.Router();
const jwt = require('jsonwebtoken');
const user = require('../models/user_model');
const auth = require('./auth');

router.get('/', auth, (request, response) => {
    const id = request.user.idcard

    user.getByCardId(id, function(err, dbResult) {
      if(err){
        response.status(500).json({ message: 'Internal server error', err });
      }
      else {
        dbObject = dbResult[0];
        if(dbObject === undefined) {
          response.status(404).json({ message: 'No user by that id' });
        }
        else {
          response.status(200).json({ message: 'Fetched user', user: dbObject });
        }
      }
    });
  }
);

router.post('/',
  function(request, response) {
    if(!request.body.fname || !request.body.lname) {
      response.status(400).send({ message: 'One or more empty fields' });
    }
    else {
      user.add(request.body, function(err, dbResult) {
        if(err){
          response.status(500).json({ message: 'Internal server error', err });
        }
        else {
          response.status(201).json({ message: 'Added user', dbResult });
        }
      })
    }
  }
);

router.put('/:id',
  function(request, response) {
    if(!request.body.fname || !request.body.lname) {
      response.status(400).send({message: 'One or more empty fields'});
    }
    else {
      user.update(request.params.id, request.body, function(err, dbResult) {
        if(err){
          response.json({ message: 'Internal server error', err });
        }
        else {
          if(dbResult.affectedRows == 0) {
            response.status(404).json({ message: 'No user by that id', dbResult })
          }
          else {
            response.status(200).json({ message:'Updated user', dbResult });
          }
        }
      })
    }
  }
);

router.delete('/:id',
  function(request, response) {
    user.delete(request.params.id, function(err, dbResult) {
      if(err){
        response.status(500).json({ message: 'Internal server error', err });
      }
      else {
        if(dbResult.affectedRows == 0) {
          response.status(404).json({ message: 'No user by that id', dbResult })
        }
        else {
          response.status(200).json({ message: 'Deleted user', dbResult });
        }
      }
    });
  }
)

module.exports = router;
