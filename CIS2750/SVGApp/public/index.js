
// Put all onload AJAX calls here, and event listeners
$(document).ready(function() {
    // On page-load AJAX Example
    $.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything 
        url: '/getSVGJSON',   //The server endpoint we are connecting to
        data: {
            fileName: "./uploads/rects.svg"
        },
        success: function (data) {
            
        },
        fail: function(error) {
            // Non-200 return, do something with error
            // $('#blah').html("On page load, received error from server");
            alert(error)
            console.log(error); 
        }
    });

    $.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything 
        url: '/getAllFiles',   //The server endpoint we are connecting to
        data: {
            name1: "Value 1",
            name2: "Value 2"
        },
        success: function (data) {
            $('#blah').html("got from server: " + data);
            data.forEach(elem => {
                let test = alterData(elem);
                console.log("received: " + test);
            });

        },
        fail: function(error) {
            // Non-200 return, do something with error
            // $('#blah').html("On page load, received error from server");
            console.log(error); 
            alert(error)
        }
    });

    dropDown();

    // Event listener form example , we can use this instead explicitly listening for events
    // No redirects if possible
    $('#someform').submit(function(e){
        $('#chicken').html("Form has data: "+$('#entryBox').val());
        e.preventDefault();
        //Pass data to the Ajax call, so it gets passed to the server
        $.ajax({
            //Create an object for connecting to another waypoint
        });
    });
});

function alterData(fileName) {
    let data = {
        file: fileName
    }
    $.ajax({
        type: "get",
        dataType: "json",
        url: 'getSVGJSON',
        data: {
            file: "./uploads/" + fileName
        },
        success: function (data) {
            if ($.isEmptyObject(data)) {
                console.log("Not going to display " + fileName + " since it is not valid.");
                return;
            }
            let root = document.getElementById('fileDisplay');
            let row = document.createElement("tr");
            let rects = document.createElement("td");
            let circ = document.createElement("td");
            let path = document.createElement("td");
            let group = document.createElement("td");

            let imgCell = document.createElement("td");
            let img = document.createElement("img");
            let link = document.createElement("a");
            let imgLink = document.createElement("a");

            let linkCell = document.createElement("td");
            let h = document.createTextNode(fileName);
            link.appendChild(h);

            let fileURL = "./uploads/" + fileName
            link.href = fileURL;
            imgLink.href = fileURL;
            imgLink.id = "img";
            img.src = fileURL;

            imgLink.appendChild(img);

            imgCell.appendChild(imgLink);
            linkCell.appendChild(link);

            let fileSize = document.createElement("td");
            fileSize.id = fileURL;
            getFileSize(fileURL);

            rects.innerHTML = data.numRect;
            circ.innerHTML = data.numCirc;
            path.innerHTML = data.numPaths;
            group.innerHTML = data.numGroups;

            row.append(imgCell, linkCell, fileSize, rects, circ, path, group);
            root.append(row);
        },
        fail: function (error) {
            console.log("Received error when getting SVGtoJSON: " + error);
            alert("Received error when getting SVGtoJSON: " + error);
        }

    })
    return fileName;
}

//Function for changing the file size of the imaage in the file table
function getFileSize(fileURL) {
    $.ajax({
        type: "get",
        dataType: "json",
        url: '/getFileSize',
        data: {
            file: fileURL
        },
        success: data => {
            let cell = document.getElementById(fileURL);
            let stringSize = Math.round(data.size / 1000);
            stringSize += "KB";
            cell.innerHTML = stringSize;
        },
        fail: err => {
            console.log("Error occured in getting the file size: " + err);
            alert("Error occured in getting the file size: " + err);
        }
    });
}

//Create the drop down to choose which file they wanat to view
function dropDown() {
    $.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything 
        url: '/returnValidSVGFiles',   //The server endpoint we are connecting to
        
        success: function (data) {
            let root = document.getElementById('files');
            let files = data.validFiles;
            files.forEach(elem => {
                let option = document.createElement("option");
                option.value = elem;
                option.innerHTML = elem;
                root.appendChild(option);
            });

        },
        fail: function (error) {
            // Non-200 return, do something with error
            // $('#blah').html("On page load, received error from server");
            console.log(error);
            alert(error)
        }
    });
}

//Call the display function for the selected file
function displayDetails() {
    let root = document.getElementById('files');
    let val = $("#files :selected").text();
    // if ($(val).isEmptyObject) {
    //     console.log("there is none selected.");
    //     return;
    // }
    displayIndividualDetails(val);
}

//Display all the details for the svg file selected
function displayIndividualDetails(file) {
    console.log("going to display values for the file: [" + file + "]");
    let root = document.getElementById('individualDetails');
    while (root.hasChildNodes()) {
        root.removeChild(root.firstChild);
    }
    let rootAttr = document.getElementById('attributeTable');
    while (rootAttr.hasChildNodes()) {
        rootAttr.removeChild(rootAttr.firstChild);
    }
    if (file == "") {
        return;
    }

    //row[0] => image
    let imgRow = document.createElement("tr");
    let imgData = document.createElement("td");
    let img = document.createElement("img");
    img.align = "middle";
    img.src = "./uploads/" + file;
    img.height = 300;
    imgData.appendChild(img);
    imgRow.append(imgData);
    root.appendChild(imgRow);
    imgData.colSpan = "4";

    //row[1] => title and description placeholders
    let titAndDesc = document.createElement("tr");
    let title = document.createElement("td");
    title.innerHTML = "Title";
    let desc = document.createElement("td");
    desc.innerHTML = "Description";
    titAndDesc.append(title, desc);
    root.appendChild(titAndDesc);
    desc.colSpan = "2";
    title.colSpan = "2";

    //row[2] => title and description strings
    let titleDescRow = document.createElement("tr");
    let titleValueCell = document.createElement("td");
    let descValueCell = document.createElement("td");
    getTitle(file, titleValueCell);
    getDescription(file, descValueCell);
    titleDescRow.append(titleValueCell, descValueCell);
    root.appendChild(titleDescRow);
    descValueCell.colSpan = "2";
    titleValueCell.colSpan = "2";

    //row[3] => component, summary, other attributes placeholder titles
    let headerRow = document.createElement("tr");
    let comp = document.createElement("td");
    let summ = document.createElement("td");
    let otherAttr = document.createElement("td");
    comp.innerText = "Components";
    summ.innerText = "Summary";
    otherAttr.innerText = "Other Attributes";
    headerRow.append(comp, summ, otherAttr);
    root.appendChild(headerRow);
    comp.colSpan = "2";

    addComponentDetails(file, root);
}

//Get the title of the svg file, then paste it in the cell
function getTitle(file, titleCell) {
    $.ajax({
        type: "get",
        dataType: "json",
        url: '/getTitle',
        data: {
            fileName: file
        },
        success: data => {
            console.log("title in get functtion.");
            console.log(data.title);
            let title = data;
            let emptyStr = "No title";
            console.log(title);
            if (title.title == "") {
                console.log("Setting innerHTML for title to: " + emptyStr);
                titleCell.innerHTML = emptyStr;
            }
            titleCell.innerHTML = title.title
        },
        fail: err => {
            console.log("Error occured in getting the title: " + err);
            alert("Error occured in getting the title: " + err);
        }
    });
}

//Get the description of the svg file, then paste it in the cell
function getDescription(file, descCell) {
    $.ajax({
        type: "get",
        dataType: "json",
        url: '/getDescription',
        data: {
            fileName: file
        },
        success: data => {
            let desc = data;
            console.log("tthe description is: ");
            console.log(desc.desc);
            if (desc.desc == "") {
                console.log("displayingg no desc.\n");
                descCell.innerHTML = "No desc.";
            }
            descCell.innerHTML = desc.desc;
        },
        fail: err => {
            console.log("Error occured in getting the title: " + err);
            alert("Error occured in getting the title: " + err);
        }
    });
}

//List off all the rectangles, paths, circles, and groups that are in the svg file
function addComponentDetails(file, root) {
    $.ajax({
        type: "get",
        dataType: "json",
        url: '/getRects',
        data: {
            fileName: file
        },
        success: data => {
            let rects = JSON.parse(data.rects);
            let i = 1;
            rects.forEach(elem => {
                let main = document.createElement("tr");
                let name = document.createElement("td");
                name.innerText = "Rect " + i;
                let dataCur = elem;
                let dataCell = document.createElement("td");
                dataCell.innerHTML = "x: " + dataCur.x + dataCur.units + " y: " + dataCur.y + dataCur.units + " width: " + dataCur.w + dataCur.units + " height: " + dataCur.h + dataCur.units;
                let attributes = document.createElement("td");
                attributes.innerHTML = "  " + dataCur.numAttr + "  ";

                let showAttr = document.createElement("input");
                showAttr.type = "button";
                showAttr.value = "Show";
                showAttr.onclick = function () {
                    showAttributes(dataCur, file, "r");
                };
                attributes.appendChild(showAttr);

                main.append(name, dataCell, attributes);
                root.appendChild(main);
                dataCell.colSpan = "2";
                i++;
            });
        },
        fail: err => {
            console.log("Error occured in displaying details for rectangles: " + err);
            alert("Error occured in displaying details for rectangles: " + err);
        }
    });

    $.ajax({
        type: "get",
        dataType: "json",
        url: '/getCircs',
        data: {
            fileName: file
        },
        success: data => {
            let circ = JSON.parse(data.circ);
            let i = 1;
            circ.forEach(elem => {
                let main = document.createElement("tr");
                let name = document.createElement("td");
                name.innerText = "Circle " + i;
                let dataCur = elem;
                let dataCell = document.createElement("td");
                dataCell.innerHTML = "x: " + dataCur.cx + dataCur.units + " y: " + dataCur.cy + dataCur.units + " r: " + dataCur.r + dataCur.units;
                let attributes = document.createElement("td");
                attributes.innerHTML = "  " + dataCur.numAttr + "  ";

                let showAttr = document.createElement("input");
                showAttr.type = "button";
                showAttr.value = "Show";
                showAttr.onclick = function () {
                    showAttributes(dataCur, file, "c");
                };
                attributes.appendChild(showAttr);

                main.append(name, dataCell, attributes);
                root.appendChild(main);
                dataCell.colSpan = "2";
                i++;
            });
        },
        fail: err => {
            console.log("Error occured in displaying details for circles: " + err);
            alert("Error occured in displaying details for circles: " + err);
        }
    });

    $.ajax({
        type: "get",
        dataType: "json",
        url: '/getPaths',
        data: {
            fileName: file
        },
        success: data => {
            let paths = JSON.parse(data.paths);
            let i = 1;
            paths.forEach(elem => {
                let main = document.createElement("tr");
                let name = document.createElement("td");
                name.innerText = "Path " + i;
                let dataCur = elem;
                let dataCell = document.createElement("td");
                dataCell.innerHTML = "data: " + dataCur.d;
                let attributes = document.createElement("td");
                attributes.innerHTML = "  " + dataCur.numAttr + "  ";

                let showAttr = document.createElement("input");
                showAttr.type = "button";
                showAttr.value = "Show";
                showAttr.onclick = function () {
                    showAttributes(dataCur, file, "p");
                };
                attributes.appendChild(showAttr);

                main.append(name, dataCell, attributes);
                root.appendChild(main);
                dataCell.colSpan = "2";
                i++;
            });
        },
        fail: err => {
            console.log("Error occured in displaying details for paths: " + err);
            alert("Error occured in displaying details for paths: " + err);
        }
    });

    $.ajax({
        type: "get",
        dataType: "json",
        url: '/getGroups',
        data: {
            fileName: file
        },
        success: data => {
            let groups = JSON.parse(data.groups);
            let i = 1;
            groups.forEach(elem => {
                let main = document.createElement("tr");
                let name = document.createElement("td");
                name.innerText = "Group " + i;
                let dataCur = elem;
                let dataCell = document.createElement("td");
                dataCell.innerHTML = dataCur.children + " child elements";
                let attributes = document.createElement("td");
                attributes.innerHTML = "  " + dataCur.numAttr + "  ";

                let showAttr = document.createElement("input");
                showAttr.type = "button";
                showAttr.value = "Show";
                showAttr.onclick = function () {
                    showAttributes(dataCur, file, "g");
                };
                attributes.appendChild(showAttr);

                main.append(name, dataCell, attributes);
                root.appendChild(main);
                dataCell.colSpan = "2";
                i++;
            });
        },
        fail: err => {
            console.log("Error occured in displaying details for paths: " + err);
            alert("Error occured in displaying details for paths: " + err);
        }
    });
}

//Display the list of all the attributes.
function showAttributes(JSONString, fileName, type) {
    let root = document.getElementById('attributeTable');
    while (root.hasChildNodes()) {
        root.removeChild(root.firstChild);
    }
    let URl = '';
    if (type === "r") {
        URL = '/JSONtoAttrsRect';
    } else if (type === "c") {
        URL = '/JSONtoAttrsCirc';
    } else if (type === "p") {
        URL = '/JSONtoAttrsPath';
    } else if (type === "g") {
        URL = '/JSOntoAttrsGroup';
    }
    $.ajax({
        type: "get",
        dataType: "json",
        url: URL,
        data: {
            string: JSON.stringify(JSONString),
            fileName: fileName
        },
        success: data => {
            let attributes = JSON.parse(data.attrs);
            if (attributes.length > 0) {
                let titleRow = document.createElement("tr");
                let title = document.createElement("td");
                let b = document.createElement("b");
                b.innerHTML = "Chosen attributes are:";
                title.colSpan = "2";
                title.append(b);
                titleRow.append(title);
                root.appendChild(titleRow);

                let headerRow = document.createElement("tr");
                let headerName = document.createElement("td");
                let text = document.createElement("h5");
                text.innerHTML = "Name";
                headerName.append(text);
                let headerValue = document.createElement("td");
                text = document.createElement("h5");
                text.innerHTML = "Value";
                headerValue.append(text);
                headerRow.append(headerName, headerValue);
                root.appendChild(headerRow);

                attributes.forEach(elem => {
                    let row = document.createElement("tr");
                    let name = document.createElement("td");
                    name.innerHTML = elem.name;
                    let value = document.createElement("td");
                    value.innerHTML = elem.value;
                    row.append(name, value);
                    root.appendChild(row);
                });
            }
        },
        fail: err => {
            console.log("Error occured in getting attributes: " + err);
            alert("Error occured in getting attributes: " + err);
        }
    });
}