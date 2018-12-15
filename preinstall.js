const exec = require('child_process').execSync;
exec(`npm install node-gyp`, {
  stdio: [0, 1, 2]
});

exec(`./node_modules/.bin/node-gyp rebuild`, {
  env: Object.assign(process.env, {
    PKG_CONFIG_PATH: `${process.env.INIT_CWD}/vendor/imagemagick/lib/pkgconfig/`
  }),
  stdio: [0, 1, 2]
});

exec(`cp "${__dirname}/Apple Chancery.ttf" build/Release/`, { stdio: [0, 1, 2] })

console.log("Copying dependencies: ");
['libIlmImf', 'libwmflite', 'libHalf', 'libIex', 'libIlmThread'].forEach(lib => {
  exec(`cp /usr/lib/x86_64-linux-gnu/${lib}* ${__dirname}/build/Release/`);
});
