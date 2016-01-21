var express = require('express');
var morgan = require('morgan');
var fs = require('fs');
var url = require('url');

var pathPrefix = './files';
var app = express();

app.use(morgan('dev'));

app.use( function(req, res, next) {
  var path = req.path;
  path = url.parse(path).pathname;
  path = pathPrefix + decodeURIComponent(path);
  res.locals.decodedPath = path;
  next();
});

app.get(/.*/, function(req, res, next) {
  if (!req.xhr) {
    return next();
  }
  var path = res.locals.decodedPath;

  fs.stat(path, function(error, stats) {
    if (error && error.code == "ENOENT") {
      next();
      return;
    }

    if (error) {
      return next(error);
    }

    if (stats.isDirectory()) {
      fs.readdir(path, function(error, files) {
        if (error) return next(error);

        files = files.map(function(file) {
          var stat = fs.statSync(path + "/" + file);
          return {
            name: file,
            isFolder: stat.isDirectory(),
          }
        });
        if (path != pathPrefix + "/") {
          files.unshift({name: '..', isFolder: true});
        }
        res.json(files);
      });
      return;
    }
    next();
  });
});

app.put(/.*/, function(req, res, next) {
  var path = res.locals.decodedPath;
  var outStream = fs.createWriteStream(path);
  outStream.on("error", function(error) {
    next(error);
  });
  outStream.on("finish", function() {
    res.status(204).send();
  });
  req.pipe(outStream);
});

app.delete(/.*/, function(req, res, next) {
  var path = res.locals.decodedPath;
  var respondErrorOrNothing = function(error) {
    if (error) {
      return next(error);
    }
    return res.status(204).send();
  }

  fs.stat(path, function(error, stats) {
    if (error && error.code == "ENOENT") {
      return res.status(204).send();
    }
    if (error) {
      return next(error);
    }
    if (stats.isDirectory()) {
      fs.rmdir(path, respondErrorOrNothing)
    } else {
      fs.unlink(path, respondErrorOrNothing);
    }
  });
});

app.mkcol(/.*/, function(req, res, next) {
  var path = res.locals.decodedPath;
  var respondErrorOrNothing = function(error) {
    if (error) {
      return next(error);
    }
    return res.status(204).send();
  }

  fs.stat(path, function(error, stats) {
    if (error && error.code != "ENOENT") {
      return next(error);
    }
    if (stats && stats.isDirectory()) {
      return res.status(204).send();
    }
    fs.mkdir(path, respondErrorOrNothing);
  })
});

app.use(express.static(pathPrefix));

app.use( function(req, res, next) {
  res.status(404);
  res.send("Page not found");
})
app.listen(8000);

