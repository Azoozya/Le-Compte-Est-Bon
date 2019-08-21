// -------------------------------------------------------------------------------
var addon = require('./Lceb');
  var express = require('express');
  var util = require('util');
  var fs = require('fs');

  var handle = express();
  handle.use(express.json()); // for parsing application/json
  handle.use(express.urlencoded({ extended: true })); // for parsing application/x-www-form-urlencoded

  handle.post('/', function(req,res,next) {
  console.log('Connection received');
  next();
  },function(req,res)
  {
    //On initialise le nom du fichier
    var file = "Input.json";

    //On met le contenu du req.body dans un fichier
    var fd = fs.openSync(file,'w');
    fs.writeFileSync(file,JSON.stringify(req.body));
    fs.close(fd, (err) => {if (err) throw err;});

    //On appelle l'addon
    addon.Lceb();

    //On lit le nouveau fichier string.json
    var to_return = "Possibilities.json";
    console.log('Request Ended');
    res.sendFile(to_return);
  }           );

  handle.listen(65535,function()
  {
    console.log('Listening on port 65535');
  }                             );


////////// Dès que l'addon sera capable de transformer un long en char*
    // var addon = require('./Lceb');
    // var express = require('express');
    // var util = require('util');
    // var fs = require('fs');
    //
    // var handle = express();
    // handle.use(express.json()); // for parsing application/json
    // handle.use(express.urlencoded({ extended: true })); // for parsing application/x-www-form-urlencoded
    //
    // function getRandomInt(max) {
    //   return Math.floor(Math.random() * Math.floor(max));
    // };
    //
    // handle.post('/', function(req,res,next) {
    // console.log('Connection received');
    // next();
    // },function(req,res)
    // {
    //   //On génère le nom de fichier
    //   var number = getRandomInt(100000)+100000;
    //   var string = number.toString();
    //   console.log('%d',number);
    //
    //   //On met le contenu du req.body dans un fichier
    //   var fd = fs.openSync(string,'w');
    //   fs.writeFileSync(string,JSON.stringify(req.body));
    //   fs.close(fd, (err) => {if (err) throw err;});
    //
    //   //On appelle l'addon
    //   addon.Lceb(number);
    //
    //   //On renvoie le nouveau fichier string.json
    //   string = "./"+string+".json";
    //   res.sendFile(string);
    //   console.log('Request Ended');
    // }           );
    //
    // handle.listen(65535,function()
    // {
    //   console.log('Listening on port 65535');
    // }                             );
