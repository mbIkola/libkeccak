module.exports = function(config) {

    config.set({
        basePath : "./",
        frameworks: ['browserify', 'mocha',  'chai'],
        files: [
            {pattern: 'test/**/*.js', watched: true, served: true, included: true},
            {pattern: 'dist/**/*.*', watched: true, served: true, included: false, nocache: true},
            {pattern: 'test/**/*.json', watched: true, served: true, included: false, nocache: true}
        ],

        preprocessors: {
            'test/**/*.js' : ['browserify']
        },
        mime : {
            'application/wasm': ['wasm']
        },
        browserify: {
            debug: true,
            transform: ['brfs']
        },
        proxies : {
            '/wa/' : '/base/dist/wa/',
            '/emscripten/' : '/base/dist/emscripten/',
            '/data/': '/base/test/data/',
            '/keccak.wasm' : '/base/dist/emscripten/keccak.wasm' // specially for fucking emcc wrappers
        },
        reporters: ['progress'],
        port: 9876,
        colors: true,

        // see whats going on
        logLevel: config.LOG_DEBUG,
        browsers: ['ChromeHeadless'],

        // use autoWatch=true for quick and easy test re-execution once files change
        autoWatch: true,
        concurrency: Infinity
    });
}
