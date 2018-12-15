{
  "targets": [
     {
      "target_name": "Edit",
          "sources": [
            "native/3-edit.cc"
      ],
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions' ],
      'include_dirs': ["<!@(node -p \"require('node-addon-api').include\")"],
      'dependencies': ["<!(node -p \"require('node-addon-api').gyp\")"],
      'conditions': [
        ['OS=="mac"', {
          "xcode_settings": {
            "CLANG_CXX_LIBRARY": "libc++",
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
            'MACOSX_DEPLOYMENT_TARGET': '10.7',
            'OTHER_CFLAGS': [
                '<!@(pkg-config --cflags Magick++)'
            ],
            'OTHER_CPLUSPLUSFLAGS': [
                '<!@(pkg-config --cflags Magick++)',
              '-std=c++11',
              '-stdlib=libc++',
            ],
          },
          "libraries": [
              '<!@(pkg-config --libs Magick++)',
          ],
          'cflags': [
              '<!@(pkg-config --cflags Magick++)'
          ]
        }],
        ['OS=="linux" or OS=="solaris" or OS=="freebsd"', { # not windows not mac
          "libraries": [
            "-L/usr/lib/x86_64-linux-gnu",
            "-Wl,-rpath,$(INIT_CWD)/vendor/imagemagick/lib,-rpath,/usr/lib/x86_64-linux-gnu",
            '<!@(pkg-config --libs Magick++)',
            "-L$(INIT_CWD)/vendor/imagemagick/lib"
          ],
          'cflags': [
            '<!@(pkg-config --cflags Magick++)'
          ],
          'cflags_cc': ['<!@(pkg-config --cflags Magick++)'],
          'include_dirs+': [
              '$(INIT_CWD)/vendor/imagemagick/include/ImageMagick-7',
          ]
        }]
      ]
    }
  ]
}
