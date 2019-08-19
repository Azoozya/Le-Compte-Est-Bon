// -------------------------------------------------------------------------------
var addon = require('Lceb');
  var express = require('express');
  var util = require('util');
  var handle = express();
  app.use(express.json()) // for parsing application/json
  app.use(express.urlencoded({ extended: true })) // for parsing application/x-www-form-urlencoded
  function getRandomInt(max) {
    return Math.floor(Math.random() * Math.floor(max));
  }
  handle.post('/', function(req,res,next) {
  console.log('Connection received');
  next();
  },function(req,res)
  {
    //On génère le nom de fichier
    var chameau =  getRandomInt(10000);
    console.log('%d',chameau);
    //On crée le fichier avec le nom  qu'on vient de générer
    fs.open('dromadaire','w',(err,fd,req,res)=>
    {
      console.log(req.body);
      res.json(req.body);
      console.log('Request Ended');
      fs.close(fd, (err) => {
          if (err) throw err;
       });
     });
     // res.end('<a href="http://www.google.com">Loutre</a>');
  });
  handle.listen(65535,function()
  {
    console.log('Listening on port 65535');
  }                             );


  var express = require('express')

  var handle = express()



  handle.post('/', function (req, res, next) {
    console.log(req.body)
    res.json(req.body)
  })
