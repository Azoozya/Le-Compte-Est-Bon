const addon = require('./build/Release/Lceb');

console.time('c++');
addon.json_gen("Input.json");
console.timeEnd('c++');
