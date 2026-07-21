import test from 'node:test';import assert from 'node:assert/strict';
process.env.DATA_ENCRYPTION_KEY='test-encryption-key';process.env.STATE_SECRET='test-state-secret';process.env.NODE_ENV='test';
const{seal,open}=await import('../server.mjs');
test('encrypted account records round trip',()=>{const value={pageId:'123',pageToken:'secret'};const encrypted=seal(value);assert.ok(!encrypted.includes('secret'));assert.deepEqual(open(encrypted),value)});
