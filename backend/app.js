var express = require('express');
var path = require('path');
var cookieParser = require('cookie-parser');
var logger = require('morgan');

var indexRouter = require('./routes/index');
var userRouter = require('./routes/user');
var loginRouter = require('./routes/login'); 
var cardRouter = require('./routes/card');
var transactionRouter = require('./routes/transaction');
var accountRouter = require('./routes/account');
var withdrawRouter = require('./routes/withdraw');


var app = express();

app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));

app.use('/', indexRouter);
app.use('/user', userRouter);
app.use('/login', loginRouter);
app.use('/card', cardRouter);
app.use('/transaction', transactionRouter);
app.use('/account', accountRouter);
app.use('/withdraw', withdrawRouter);


module.exports = app;
