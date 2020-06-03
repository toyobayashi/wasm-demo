const addon = require('./build/Release/addon.node')

const path = require('path')
const Koa = require('koa')
const KoaRouter = require('koa-router')
const koaStatic = require('koa-static')
const bodyparser = require('koa-bodyparser')

const router = new KoaRouter()
router.post('/api/secret', async (ctx) => {
  const body = ctx.request.body
  console.log('Request body: ' + body)
  const dec = addon.decryptJson(body)
  console.log('JSON:')
  console.log(dec)
  ctx.response.body = addon.encryptJson({
    code: 0,
    data: dec,
    error: null,
    message: 'OK'
  })
})

const app = new Koa()
app.use(koaStatic(path.join(__dirname, 'public')))
app.use(bodyparser({
  enableTypes: ['json', 'form', 'text']
}))
app.use(router.routes())
app.use(router.allowedMethods())

app.listen(8096, '127.0.0.1', () => {
  console.log('Server running at: http://127.0.0.1:8096')
})
