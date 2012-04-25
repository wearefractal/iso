Isolate = require '../'
Benchmark = require 'benchmark'
should = require 'should'
async = require 'async'
require 'mocha'

describe 'Isolate', ->
  describe 'benchmark', ->
    it 'should bench mad bonez', (done) ->
      @timeout '1000000000000'

      # test functions
      tiny = -> 
        a = b = 1
        return a+b
      simple = -> 
        arr = []
        arr.push "Test"
        return arr
      standard = ->
        arr = ["Tony", "Mike", "John", "Tom"]
        out = {}
        arr.forEach (el) ->
          out[el] = if el[0] is 'T' then "Starts with T" else "Is lame"
        return out
      complex = ->
        out = ""
        rand = -> (((1 + Math.random()) * 0x10000) | 0).toString 16
        out += rand() for num in [0...10000]
        return out

      atiny = (cb) -> 
        a = b = 1
        cb a+b
        return

      asimple = (cb) -> 
        arr = []
        arr.push "Test"
        cb arr
        return

      astandard = (cb) ->
        arr = ["Tony", "Mike", "John", "Tom"]
        out = {}
        add = (el, cb) ->
          out[el] = if el[0] is 'T' then "Starts with T" else "Is lame"
          cb()
        async.forEach arr, add, -> cb out
        return

      acomplex = (cb) ->
        out = ""
        rand = -> (((1 + Math.random()) * 0x10000) | 0).toString 16
        add = (el, cb) ->
          out += rand()
          cb()
        async.forEach [0...10000], add, -> cb out
        return

      tinyiso = new Isolate tiny
      simpleiso = new Isolate simple
      standardiso = new Isolate standard
      complexiso = new Isolate complex
      
      atinyiso = new Isolate atiny
      asimpleiso = new Isolate asimple
      astandardiso = new Isolate astandard
      acomplexiso = new Isolate acomplex

      suite = new Benchmark.Suite 'Isolate'
      suite.on "error", (event, bench) -> throw bench.error
      suite.on "cycle", (event, bench) -> console.log String bench
      suite.on "complete", (event, bench) -> done()

      suite.add "tiny", tiny
      suite.add "simple", simple
      suite.add "standard", standard
      suite.add "complex", complex

      suite.add "async tiny", -> atiny (s) ->
      suite.add "async simple", -> asimple (s) ->
      suite.add "async standard", -> astandard (s) ->
      suite.add "async complex", -> acomplex (s) ->

      suite.add "Isolate(tiny)", -> tinyiso.run()
      suite.add "Isolate(simple)", -> simpleiso.run()
      suite.add "Isolate(standard)", -> standardiso.run()
      suite.add "Isolate(complex)", -> complexiso.run()


      suite.add "async Isolate(tiny)", -> atinyiso.run (s) ->
      suite.add "async Isolate(simple)", -> asimpleiso.run (s) ->
      suite.add "async Isolate(standard)", -> astandardiso.run (s) ->
      suite.add "async Isolate(complex)", -> acomplexiso.run (s) ->

      suite.run async: true, queued: true