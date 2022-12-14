// Modules to control application life and create native browser window
const {app, BrowserWindow, Notification, ipcMain} = require('electron')
const Jimp = require('jimp')
const path = require('path')

function createWindow () {
  // Create the browser window.
  const mainWindow = new BrowserWindow({
    width: 1000,
    height: 600,
    webPreferences: {
      preload: path.join(__dirname, 'output.js'),
      preload: path.join(__dirname, 'preload.js')
    }
  })

  // and load the index.html of the app.
  mainWindow.loadFile('electron/index.html')

  // Open the DevTools.
  // mainWindow.webContents.openDevTools()
  
  ipcMain.on('save-img', (event, filePath, img, rows, cols) => {

    const webContents = event.sender
    const win = BrowserWindow.fromWebContents(webContents)
    console.log("save-img main.js", filePath)
    console.log(img, rows, cols);
    let fullPath = path.join(filePath, 'output.jpeg');
    
    console.log(fullPath)

    new Jimp({
      width: cols,
      height: rows,
      data: Buffer.from(img)
    }).write(fullPath);


  new Notification({title: "saved file", body: `file saved to ${fullPath}`}).show();
  })
}

// This method will be called when Electron has finished
// initialization and is ready to create browser windows.
// Some APIs can only be used after this event occurs.
app.commandLine.appendSwitch('enable-features','SharedArrayBuffer')


const NOTIFICATION_TITLE = 'Basic Notification'
const NOTIFICATION_BODY = 'Notification from the Main process'


app.whenReady().then(() => {
    
  
  createWindow()

  app.on('activate', function () {
    // On macOS it's common to re-create a window in the app when the
    // dock icon is clicked and there are no other windows open.
    if (BrowserWindow.getAllWindows().length === 0) createWindow()
  })

})

// Quit when all windows are closed, except on macOS. There, it's common
// for applications and their menu bar to stay active until the user quits
// explicitly with Cmd + Q.
app.on('window-all-closed', function () {
  if (process.platform !== 'darwin') app.quit()
})

// In this file you can include the rest of your app's specific main process
// code. You can also put them in separate files and require them here.

app.on('convolution-ran', function (time) {
  new Notification({title: "conv-ran", body: "conv-ran-body"}).show();
})