const addon = require('./build/Release/addon');

function jsSum()
{
  let a = 3.1415926,b = 2.718;
  for (let i = 0 ; i < 100000; i++)
  {
    a += b;
  }
  let total = a;
  return total;
}

console.time('c++');
addon.sum();
console.timeEnd('c++');

console.time('js');
jsSum();
console.timeEnd('js');
