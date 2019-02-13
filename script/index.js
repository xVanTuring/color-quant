const sharp = require('sharp')
const fs = require('fs')
sharp("image.png")
    .raw()
    .toBuffer((err, buffer) => {
        fs.writeFile(`data.4.rgb`, buffer, () => {
        })
    })
sharp("image.jpg")
    .raw()
    .toBuffer((err, buffer) => {
        fs.writeFile(`data.3.rgb`, buffer, () => {
        })
    })