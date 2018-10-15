const fs = require("fs").promises;

/**
 * Test method try out the new API.
 */

// 1. Module that returns the version number of ImageMagick.
const versionFinder = require("./build/Release/Version.node");
console.info("ImageMagick Version - ", versionFinder.version());

// 2. Pass an image to get info.
(async () => {
  // Async fs.readFile - Experimental in node 10. Don't use until stable.
  // https://nodejs.org/api/fs.html#fs_fs_promises_api
  const image = await fs.readFile('logo.png');
  const infoProvider = require("./build/Release/Info.node");
  const processed = infoProvider.processImage(image);
  // JavaScript has a JSON beautifier built in. The third argument
  // is the number of characters to use as whitespace -
  // https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/JSON/stringify
  console.info(JSON.stringify(processed, 0, 2));
})();
