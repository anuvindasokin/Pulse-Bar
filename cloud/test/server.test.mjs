import test from 'node:test';import assert from 'node:assert/strict';
process.env.DATA_ENCRYPTION_KEY='test-encryption-key';process.env.STATE_SECRET='test-state-secret';process.env.NODE_ENV='test';
const{seal,open,handler}=await import('../server.mjs');
test('encrypted account records round trip',()=>{const value={pageId:'123',pageToken:'secret'};const encrypted=seal(value);assert.ok(!encrypted.includes('secret'));assert.deepEqual(open(encrypted),value)});
function response(){return{status:0,headers:{},payload:'',writeHead(status,headers){this.status=status;this.headers=headers;return this},end(payload=''){this.payload=payload;return this}}}
test('health endpoint reports service state',async()=>{const res=response();await handler({url:'/health',headers:{},method:'GET'},res);assert.equal(res.status,200);assert.equal(JSON.parse(res.payload).ok,true)});
test('customer portal renders registration and login',async()=>{const res=response();await handler({url:'/',headers:{},method:'GET'},res);assert.equal(res.status,200);assert.match(res.payload,/Create account/);assert.match(res.payload,/Log in/)});
