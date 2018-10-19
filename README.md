Image Info - N-API Tutorial
===========================
> Do not use this on production. This is is meant to be a tutorial and not a production API. The code has been intentionally minimized to ignore many error cases and uses synchronous APIs for simplification.

Sample code which introduces the N-API stable in Node.js since Node 10+. The native code is present in the native folder. The rest of the code is present to demonstrate a sample application.

Here are the native pieces and the order in which they should be read:
1) **Version** Gets the ImageMagick version as a string.
2) **Info** Gets some basic information about the image from ImageMagick.
3) **Edit** Edits an image via ImageMagick to create a new version used in the demo.

There are three versions of the sample usage app:

1) **CLI** Wonderful for testing and debugging. Also used for the C++ debugger provided for VS Code.
2) **Server** Express.js based web server hosting a sample app for the **Edit** application.
3) **Electron** Sample Electron code using the **Edit** module.

No recompilation should be needed to run the electron and the node versions. Switching node versions should not need a recompilation to utilize the API. While the API is experimental in node 8 and should not be used in production, this code works for Node 8+ (with a warning in Node 8).

## Important Scripts:
* Recompile Native code
```
node-gyp rebuild
```
* Run Electron Version
```
npm run electron
```
* Run Web server
```
npm start
```
* Run command line API tests
```
npm test
```
