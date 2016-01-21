var fileList = document.getElementById("files");
var fileContent = document.getElementById("fileContent");

var currentDirectory = "/";

function xhrRequest(url, method, callback, body) {
  var req = new XMLHttpRequest();
  req.open(method, url, true);
  req.setRequestHeader("X-Requested-With", "XMLHttpRequest");
  req.addEventListener("load", function () {
    callback.call(req, req);
  })
  req.send(body);
}

function getFileContent(url) {
  xhrRequest(url, 'GET', function () {
    var content = this.responseText;
    fileContent.innerText = content;
  });
}

function getCurrentDirectory(callback){
  xhrRequest(currentDirectory, 'GET', function () {
    var json = JSON.parse(this.responseText);
    callback.call(null, json);
  });
}

function buildLink(name) {
    var linkUrl = name;
    if (currentDirectory != "/") {
      linkUrl = currentDirectory +  "/" + linkUrl;
    } else {
      linkUrl = currentDirectory + linkUrl;
    }

    // resolve ".."
    linkUrl = linkUrl.replace(/\/([^\/]+\/\.\.)$/, "/");
    return linkUrl;
}

function isProtectedFile(name) {
  if (name == "..") return true;
  if (name.match(/^index\.*/)) return true;

  return false;
}

function updateFileList() {
  var createLinkElement = function(name) {
    var link = document.createElement("A");
    var linkUrl = buildLink(name);
    link.setAttribute("data-link", linkUrl);

    return link;
  }

  clearFileList();
  var response = getCurrentDirectory(function (json) {
    console.log(json);
    json.forEach( function (fileOrFolder) {
      var node = document.createElement("LI");
      var linkClass = fileOrFolder.isFolder ? 'fa-folder-o' : 'fa-file-o';
      var link = createLinkElement(fileOrFolder.name);

      link.setAttribute("data-is-folder", fileOrFolder.isFolder);
      link.addEventListener("click", onFileOrFolderClick);
      link.innerHTML = '<i class="fa ' + linkClass + '"></i> ' + fileOrFolder.name;
      node.appendChild(link);

      if (!isProtectedFile(fileOrFolder.name)) {
        link = createLinkElement(fileOrFolder.name);
        link.addEventListener("click", onDeleteFile);
        link.classList.add('delete');
        link.innerHTML = '<i class="fa fa-trash"></i>';
        node.appendChild(link);
      }

      fileList.appendChild(node);
    });
  });
}

function onDeleteFile(event) {
  var link = this.getAttribute("data-link");
  xhrRequest(link, 'DELETE', updateFileList);
}

function onFileOrFolderClick(event) {
  var isFolder = this.getAttribute("data-is-folder") == "true";
  var link = this.getAttribute("data-link");
  if (isFolder) {
    currentDirectory = link;
    updateFileList();
    return;
  }

  getFileContent(link);
}

function clearFileList() {
  var children = fileList.querySelectorAll("li");
  mapNodeList(children, function(i, element) {
    element.remove()});
  return;
}

function mapNodeList(nodeList, fun) {
  for (i = 0; i < nodeList.length; i++) {
    nodeList[i] = fun.call(nodeList[i], i, nodeList[i]);
  }
  return nodeList;
}

function createFolder(event) {
  event.preventDefault();

  var folder = document.getElementById("newfoldername").value;

  xhrRequest(buildLink(folder), "MKCOL", updateFileList);
  document.getElementById("newfolderform").reset();
}

function createFile(event) {
  event.preventDefault();

  var filename = document.getElementById("newfilename").value;
  var content = document.getElementById("newfilecontent").value;

  if (isProtectedFile(filename)) {
    alert("Datei darf nicht überschrieben werden!");
    return;
  }
  xhrRequest(buildLink(filename), "PUT", updateFileList, content);
  document.getElementById("newfileform").reset()
}

window.onload = updateFileList;
