/**
 * Test method try out the new API.
 */

// 1. Module that returns the version number of ImageMagick.
const versionFinder = require("./build/Release/Version.node");
console.log(versionFinder.version());

