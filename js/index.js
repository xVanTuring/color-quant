const sharp = require('sharp')
const fs = require('fs')
sharp('./adventure-animal-clouds-1557650.jpg')
  .resize(1024)
  .raw()
  .toBuffer((err, buffer) => {
    fs.writeFile('./data.rgb', buffer, () => {
      console.log('Done!')
    })
  })