const net = require('net')

const server = net.createServer(socket => {
  console.log('New client connected')

  let data = Buffer.from('')

  socket.on('data', data => {
    data = Buffer.concat([data, chunk])
    console.log(`Received data: ${data}`)
  })

  socket.on('end', () => {
    const decodedData = data.toString('utf8')
    console.log(`Received data: ${decodedData}`)
  })

  socket.on('close', () => {
    console.log('Client disconnected')
  })
})

const port = 19099
server.listen(port, () => {
  console.log(`Server listening on port ${port}`)
})
