

describe('Array', () => {
    describe('Karma Working', () => {
        it('Карма должна уметь стартовать браузер, инъектить в него чай с мочой и запускать тесты', () => {
            console.info("Running in browser (probably)" + navigator.userAgent);
            assert.equal(-1, [1, 2, 3].indexOf(4));
        });
    });
    describe('webassembly', () => {
        it("Should be able to load WASM module", (done) => {
            let responseStatus = 404;
            fetch('/wa/keccak.wasm')
                .then ( response =>  {
                    responseStatus = response.status;
                    expect(response.status).to.be.eq(200);
                    return response.arrayBuffer();
                } )
                .then( buffer => {
                    expect(buffer).to.be.instanceOf(ArrayBuffer);
                    expect(buffer.byteLength).to.be.greaterThan(0);
                    console.info("WASM module length" , buffer.byteLength);
                    return null;
                })
                .then(done, () => {
                    console.error("====== GOT " + responseStatus + " while GET /wa/keccak.wasm.");
                    console.error("====== Please ensure you have compiled it and running karma from repo root");

                    done("KARMA embedded browser doesnt serve static correctly");
                })

        });

        it("Should be able to compile and link WASM module via WebAssembly Loader", (done) => {

           const wa = require('webassembly');


           const promise = wa.load('/wa/keccak.wasm')
               .then(module => {
                   console.info("Module loaded:", module);
                   expect(module).to.not.be.an('undefined');
               }, err => {
                   console.error("====== There is an error while loading wasm module via WEBASSEMBLY loader");
                   if (err instanceof  WebAssembly.CompileError ) {
                       console.error(err.name + " " + err.message + " at " + err.fileName + " +" + err.lineNumber + ":" + err.columnNumber);
                       throw err;
                   } else if ( err instanceof  WebAssembly.LinkError ) {
                       console.error( "====== " +  err.name + " " + err.message );
                       console.error( "====== " + "Ensure you're importing all dependencies in WA ");
                       throw err;
                   } else {
                       console.error("Я не вулканолог, но скажите: а вы пробовали в исландский вулкан бросать девственниц?");
                       // probably may be linker issue
                       throw new Error('Vse, pizdos: '+ err , err);
                   }
               });

           promise.then(done, err => done(err));
        });
    });
    describe('test fixtures', () => {
        function load(fname) {
            return it("Should be able to load and parse JSON text fixtures: " + fname, (done) => {
                fetch('/data/' + fname )
                    .then(response => {
                        expect(response.status).to.be.eq(200);
                        return response.json();
                    })
                    .then(fixtures => {
                        expect(fixtures).to.be.an('array').that.not.empty;
                    }).then(done, done);


            });
        }
        load('keccak1600.json');
        load('keccakf.json');
    });

    describe('test emscripten', () => {
        var Module =  {
            ENVIRONMENT : "WEB",
            preRun: [],
            noExitRuntime : true,
            postRun: [ function() { console.error("postrun function"); }],
            wasmBinaryFile : '/base/dist/emscripten/keccak.wasm',
            printErr : (...err) => {
                console.error("======= Someting goes wrong: ", err);
                console.error("=======", err);
                done(err);
            },
            onRuntimeInitialized : void(0),
            setStatus : (statusText) => {
                if ( statusText.length > 0 ) console.info("Loading status:  " + statusText);
            }

        };
        window.Module = Module;
        var runtimeInitializedPromise = new Promise( function(resolve, reject) {
            Module.onRuntimeInitialized = resolve;
            Module.onError = reject;
        });
        function injectScript(src)  {
            return new Promise( (resolve, reject) => {
                var d = document, s = d.createElement('script');
                s.async = true; s.src = src; s.type = "text/javascript";
                s.onerror = reject; s.onload =  resolve;
                d.head.appendChild(s);
            });
        }

        console.info("====================        EMSCRIPTEN SUITE  =========================" +
                     "12345678".split(/./ig).map(a => "\r\n").join(""));


        before( (done) => {
           injectScript("/base/dist/emscripten/keccak.js")
               .then( res => runtimeInitializedPromise.then( () => {
                    console.info("Runtime initialized. ");
                    done();
               }))
               .catch( err => done(err));
        });

        it("#exportedFunctions", () => {
            let exports = ["selfTest", "selfTest2", "keccak", "keccakf", "keccak1600"];

            exports.map( expectedExportedFunction => {
                expect(Module["_" + expectedExportedFunction], "#" + expectedExportedFunction + "() doesnt exported. ")
                    .to.be.an.instanceof(Function);
            });
        });


        it("#selfTest", () => {
            // function _selfTest(isNotPizdec) returning (int)isNotPizdec if everything ok,
            // and ~isNotPizdec (bitwise not ) otherwise.
            expect(Module._selfTest(42)).to.be.eq(42);
        });

        it("#selfTest2", () => {
            // function _selfTest(isNotPizdec) returning (int)isNotPizdec if everything ok,
            // and ~isNotPizdec (bitwise not ) otherwise.
            expect(Module._selfTest2(42)).to.be.eq(42);
        });



    });
});

