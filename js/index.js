const sharp = require('sharp')
// const path = require('path')
const fs = require('fs')
sharp("image.png")
    .raw()
    .toBuffer((err, buffer) => {
        fs.writeFile(`data.4.rgb`, buffer, () => {
        })
    })
sharp("image.png")
    .removeAlpha()
    .raw()
    .toBuffer((err, buffer) => {
        fs.writeFile(`data.3.rgb`, buffer, () => {
        })
    })