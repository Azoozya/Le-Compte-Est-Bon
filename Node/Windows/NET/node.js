// -------------------------------------------------------------------------------
var addon = require('Lceb');
  var express = require('express');
  var util = require('util');
  var handle = express();
  handle.use(express.json()); // for parsing application/json
  handle.use(express.urlencoded({ extended: true })); // for parsing application/x-www-form-urlencoded
  function getRandomInt(max) {
    return Math.floor(Math.random() * Math.floor(max));
  }
  handle.post('/', function(req,res,next) {
  console.log('Connection received');
  next();
  },function(req,res)
  {
    //On génère le nom de fichier
    var number =  getRandomInt(10000);
    var string = number.toString();
    console.log('%d',number);
    //On crée le fichier avec le nom  qu'on vient de générer


    //En cas d'erreur
    fs.close(fd, (err) => {
        if (err) throw err;
                          });
    console.log('Request Ended');
    res.end('{"msg": "OK"}');
    // res.end('<a href="http://www.google.com">Loutre</a>');
  });
  handle.listen(65535,function()
  {
    console.log('Listening on port 65535');
  }                             );


  });
