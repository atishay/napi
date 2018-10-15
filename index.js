const fs = require("fs");

/**
 * Test method try out the new API.
 */

// 1. Module that returns the version number of ImageMagick.
const versionFinder = require("./build/Release/Version.node");
console.info("ImageMagick Version - ", versionFinder.version());

// 2. Pass an image to get info.
// Don't use Sync on production. This is for demo purposes.
const image = fs.readFileSync('logo.png');
const infoProvider = require("./build/Release/Info.node");
const processed = infoProvider.processImage(image);
// JavaScript has a JSON beautifier built in. The third argument
// is the number of characters to use as whitespace -
// https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/JSON/stringify
console.info(JSON.stringify(processed, 0, 2));


// 3. Async processing of an image -> image
// Using callbacks as we wait on https://github.com/nodejs/node-addon-api/issues/231
// N API is hot of the machine.
// Once that is resolved it will be async await here.
const photo = fs.readFileSync('photo.jpg');
const editor = require("./build/Release/Edit.node");
editor.edit(photo, (err, processed) => {
  fs.writeFileSync('edit.jpg', processed);
});
