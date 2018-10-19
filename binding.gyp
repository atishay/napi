{
  "targets": [
    {
      "target_name": "Version",
      "sources": [
        "native/1-version.cc"
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
            '<!@(pkg-config --libs Magick++)',
          ],
          'cflags': [
            '<!@(pkg-config --cflags Magick++)'
          ],
        }]
      ]
    }, {
      "target_name": "Info",
      "sources": [
        "native/2-info.cc"
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
            '<!@(pkg-config --libs Magick++)',
          ],
          'cflags': [
            '<!@(pkg-config --cflags Magick++)'
          ],
        }]
      ]
    }, {
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
            '<!@(pkg-config --libs Magick++)',
          ],
          'cflags': [
            '<!@(pkg-config --cflags Magick++)'
          ],
        }]
      ]
    }
  ]
}
