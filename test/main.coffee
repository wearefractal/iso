Isolate = require '../'
should = require 'should'
require 'mocha'

describe 'Isolate', ->
  describe 'constructor()', ->
    it 'should create', (done) ->
      iso = new Isolate -> 1+1
      should.exist iso
      done()

  describe 'run()', ->
    it 'should run with no scope', (done) ->
      iso = new Isolate -> 1+1
      res = iso.run()
      should.exist res
      res.should.equal 2
      done()

    it 'should run with inherited scope', (done) ->
      a = b = 1
      iso = new Isolate -> a+b
      res = iso.run()
      should.exist res
      res.should.equal 2
      done()

    it 'should run with bound scope', (done) ->
      a = b = 1
      iso = new Isolate => a+b
      res = iso.run()
      should.exist res
      res.should.equal 2
      done()

    it 'should run with callback', (done) ->
      iso = new Isolate (cb) -> cb 2
      iso.run (res) ->
        should.exist res
        res.should.equal 2
        done()