const express = require('express');
const router = express.Router();
const card = require('../models/card_model');

//Kortin lisäys
router.post('/', function (request, response) {
    const card_data = {
        cardnum: request.body.cardnum,
        pin: request.body.pin,
        attempts: request.body.attempts,
        locked: request.body.locked,
        idaccount: request.body.idaccount
    };

    if (!card_data.cardnum || !card_data.pin || !card_data.attempts || !card_data.locked || !card_data.idaccount) return response.status(400).json({error: `Kortin tiedot vaaditaan.`});

    card.add(card_data, function (err, result) {
        if (err) {
            response.status(500).json(err);
        } else {
            response.status(201).json({ message: 'Kortti lisätty onnistuneesti!', result });
        }
    });
});

//Kortin poistaminen
router.delete('/', function (request, response) {
    const cardnum = request.query.cardnum;

    if(!cardnum)  return response.status(400).json({ error: 'Kortin numero vaaditaan.' });

    card.delete(cardnum, function(err, result){
        if (err) return response.status(500).json(err);

        if (result.affectedRows === 0) return response.status(404).json({error: 'Korttia ei löydy.'})

        return response.status(200).json({message: 'Kortti poistettu onnistuneesti.'});
    })
});

//idcardin haku tietokannasta
router.get('/getidcard', function(request, response){
    const cardnum = request.query.cardnum;

    if(!cardnum) return response.status(400).json({error: 'Kortin numero vaaditaan.'});

    card.getIdCard(cardnum, function(err, result){
        if (err) response.status(500).json(err);

        if (result.length===0) {
            response.status(404).json({error: 'Korttia ei löydy.'});
        } else {
            const idcard = result[0].idcard;
            response.status(200).json({message: `idcard: ${idcard}`});
        }
    });
});

//Kortin päivitys
router.put('/update', function(request, response) {
    const idcard = request.body.idcard;

    if (!idcard) return response.status(400).json({error: "Kortin ID vaaditaan."});

    const card_data = {
        cardnum: request.body.cardnum,
        pin: request.body.pin,
        idaccount: request.body.idaccount
    };

    if(!card_data.cardnum && !card_data.pin && !card_data.idaccount) return response.status(400).json({error: "Päivitettäviä tietoja ei annettu."});

    //Tarkistetaan yritetäänkö tietoturvattuja kenttiä yrittää muuttaa
    let ignoredFields = [];
    if(request.body.attempts !== undefined)  ignoredFields.push('attempts');
    if(request.body.locked !== undefined)  ignoredFields.push('locked');

    card.update(idcard, card_data, function(err, result) {
        if (err) {
            response.status(500).json(err);
        } else if (result.affectedRows === 0) {
            response.status(404).json({ message: 'Korttia ei löydy.' });
        } else {
            let responseMessage = {message: 'Kortti päivitetty onnistuneesti!'}
            if (ignoredFields.length > 0) responseMessage.warning = `Seuraavia kenttiä ei päivitetty: ${ignoredFields.join(", ")}`;

            response.status(200).json(responseMessage);
        }
    });
});

//Kortin lukitus statuksen tarkistus
router.get('/locked', function (request, response){
    const cardnum = request.query.cardnum;

    if(!cardnum) return response.status(400).json({error: 'Kortin numero vaaditaan.'});
    

    card.checkLockedStatus(cardnum, (err, result)=> {
        if (err) return response.status(500).json(err);
        
        if(result.length===0) return response.status(404).json({error: 'Korttia ei löydy.'});
        
        const locked = result[0].locked;
        return response.status(200).json({message: `Kortin status: ${locked}` });
    });
});

//Kortin lukitus
router.put('/lock', function (request, response){
    const cardnum = request.body.cardnum;

    if(!cardnum) return response.status(400).json({error: 'Kortin numero vaaditaan.'});
    
    //Tarkistetaan onko kortti jo lukittu
    card.checkLockedStatus(cardnum, (err, result)=> {
        if (err) return response.status(500).json(err);
        
        if(result.length===0) return response.status(404).json({error: 'Korttia ei löydy.'});
        
        const locked = result[0].locked;

        if(locked === 1) return response.status(409).json({error: 'Kortti on jo lukittu.'});

        //Kortin lukitseminen
        card.lockCard(cardnum, function(err, result){
            if (err) return response.status(500).json(err);
            if (result.affectedRows === 0) return response.status(404).json({ error: 'Korttia ei löydy.' });
            
            response.status(200).json({message: `Kortti lukittu.` });       
        });
    });
});

//Kortin avaaminen
router.put('/unlock', function(request, response) {
    const cardnum = request.body.cardnum;

    if (!cardnum) return response.status(400).json({ error: 'Kortin numero vaaditaan.' });

    // Lukituksen poistaminen
    card.unlockCard(cardnum, function(err) {
        if (err) return response.status(500).json(err);

        // Yritysten määrän nollaus
        card.resetAttempts(cardnum, function(err) {
            if (err) return response.status(500).json(err);

            // Haetaan päivitetty yritysten määrä
            card.getAttempts(cardnum, function(err, result) {
                if (err) return response.status(500).json(err);
                
                if (result.length === 0) return response.status(404).json({ error: 'Korttia ei löydy.' });
                
                const attempts = result[0].attempts;
                response.status(200).json({ message: `Kortti avattu ja yritykset nollattu. Yritysten määrä: ${attempts}` });
            });
        });
    });
});


//Kortin pin koodin syöttöyrityksien tarkistus
router.get('/attempts', function (request, response) {
    // Hae kortin ID requestin body:stä
    const cardnum = request.query.cardnum;

    if (!cardnum) return response.status(400).json({ error: 'Kortin numero vaaditaan.' });

    card.getAttempts(cardnum, (err, result) => {
        if (err) return response.status(500).json(err); // Palautetaan virhe

        if (result.length === 0) return response.status(404).json({ error: 'Korttia ei löydy.' });
        
        const attempts = result[0].attempts;
        response.status(200).json({ message: `Yritysten määrä: ${attempts}` });
    });
});


module.exports = router;
