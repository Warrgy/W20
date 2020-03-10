
// Put all onload AJAX calls here, and event listeners
$(document).ready(function() {
    // On page-load AJAX Example
    // $.ajax({
    //     type: 'get',            //Request type
    //     dataType: 'json',       //Data type - we will use JSON for almost everything 
    //     url: '/getSVGJSON',   //The server endpoint we are connecting to
    //     data: {
    //         fileName: "./uploads/rects.svg"
    //     },
    //     success: function (data) {
            
    //     },
    //     fail: function(error) {
    //         // Non-200 return, do something with error
    //         // $('#blah').html("On page load, received error from server");
    //         alert(error)
    //         console.log(error); 
    //     }
    // });

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
    let editAttr = document.getElementById('editAttribute');
    while (editAttr.hasChildNodes()) {
        editAttr.removeChild(editAttr.firstChild);
    }
    editAttr.style.border = "none";
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

function addShapeButton(file, root) {
    let row = document.createElement("tr");
    row.align = "center";
    let data = document.createElement("td");
    data.colSpan = "4";

    for (let i = 0; i < 4; i++) {
        let input = document.createElement("input");
        input.type = "button";
        input.padding = 1;
        input.align = "center";
        switch (i) {
            case 0:
                input.value = "Add Rectangle";
                input.onclick = () => {
                    removeExtraChildrenViaPage();
                    createRectangle(file);
                }
                break;
            case 1:
                input.value = "Add Circle";
                input.onclick = () => {
                    removeExtraChildrenViaPage();
                    createCircle(file);
                }
                break;
            case 2:
                input.value = "Add Path";
                input.onclick = () => {
                    removeExtraChildrenViaPage();
                    createPath(file);
                }
                break;
            case 3:
                input.value = "Add Group";
                input.onclick = () => {
                    removeExtraChildrenViaPage();
                    createGroup(file);
                }
                break;
        }
        data.append(input);
    }

    row.append(data);
    root.appendChild(row);
}

function createPath(file) {
    let root = document.getElementById('AddShape');

    //Make circle header
    let header = document.createElement('h4');
    header.innerHTML = "Path";
    header.align = "center";
    root.appendChild(header);

    //cx value
    let d = document.createElement("input");
    d.type = "text";
    d.placeholder = "Enter data value";
    root.appendChild(d);

    //Button
    let btn = document.createElement("input");
    root.append(btn);
    btn.type = "button";
    btn.value = "Submit";
    btn.onclick = () => {
        addPath(file, JSON.stringify({
            d: d.value
        }));
        removeExtraChildrenViaPage();
    }
}

function addPath(file, JSONPath) {
    $.ajax({
        type: "get",
        dataType: "json",
        url: '/addShape',
        data: {
            fileName: file,
            shape: JSONPath,
            type: "path"
        },
        success: data => {
            if (data.sent == true) {
                window.location.reload();
            } else {
                console.log("Issue in adding the path.");
            }
        },
        fail: err => {
            console.log("Error occured in adding the path: " + err);
            alert("Error occured in adding the path: " + err);
        }
    });
}

function createCircle(file) {
    let root = document.getElementById('AddShape');

    //Make circle header
    let header = document.createElement('h4');
    header.innerHTML = "Circle";
    header.align = "center";
    root.appendChild(header);

    //cx value
    let cx = document.createElement("input");
    cx.type = "text";
    cx.placeholder = "Enter cx value";
    root.appendChild(cx);

    //y value
    let cy = document.createElement("input");
    cy.type = "text";
    cy.placeholder = "Enter cy value";
    root.appendChild(cy);

    //y value
    let r = document.createElement("input");
    r.type = "text";
    r.placeholder = "Enter r value";
    root.appendChild(r);

    //units
    let units = document.createElement("input");
    units.type = "text";
    units.placeholder = "Units";
    root.appendChild(units);

    //Button
    let btn = document.createElement("input");
    root.append(btn);
    btn.type = "button";
    btn.value = "Submit";
    btn.onclick = () => {
        addCircle(file, JSON.stringify({
            cx: Number(cx.value),
            cy: Number(cy.value),
            r: Number(r.value),
            units: units.value
        }));
        removeExtraChildrenViaPage();
    }
}

function addCircle(file, circJSON) {
    $.ajax({
        type: "get",
        dataType: "json",
        url: '/addShape',
        data: {
            fileName: file,
            shape: circJSON,
            type: "circ"
        },
        success: data => {
            if (data.sent == true) {
                window.location.reload();
            } else {
                console.log("Issue in adding the circle.");
            }
        },
        fail: err => {
            console.log("Error occured in adding the circle: " + err);
            alert("Error occured in adding the circle: " + err);
        }
    });
}

function createRectangle(file) {
    let root = document.getElementById('AddShape');

    //Make rectangle header
    let header = document.createElement('h4');
    header.innerHTML = "Rectangle";
    header.align = "center";
    root.appendChild(header);

    //x value
    let x = document.createElement("input");
    x.type = "text";
    x.placeholder = "Enter x value";
    root.appendChild(x);

    //y value
    let y = document.createElement("input");
    y.type = "text";
    y.placeholder = "Enter y value";
    root.appendChild(y);

    //width
    let w = document.createElement("input");
    w.type = "text";
    w.placeholder = "Enter width";
    root.appendChild(w);

    //height
    let h = document.createElement("input");
    h.type = "text";
    h.placeholder = "Enter height";
    root.appendChild(h);

    //units
    let units = document.createElement("input");
    units.type = "text";
    units.placeholder = "Units";
    root.appendChild(units);

    //Button
    let btn = document.createElement("input");
    root.append(btn);
    btn.type = "button";
    btn.value = "Submit";
    btn.onclick = () => {
        addRectangle(file, JSON.stringify({
            x: Number(x.value),
            y: Number(y.value),
            w: Number(w.value),
            h: Number(h.value),
            units: units.value
        }));
        removeExtraChildrenViaPage();
    }
}

function addRectangle(file, JSONRect) {
    $.ajax({
        type: "get",
        dataType: "json",
        url: '/addShape',
        data: {
            fileName: file,
            shape: JSONRect,
            type: "rect"
        },
        success: data => {
            if (data.sent == true) {
                window.location.reload();
            } else {
                console.log("Issue in adding the rectangle.");
            }
        },
        fail: err => {
            console.log("Error occured in adding the rectangle: " + err);
            alert("Error occured in adding the rectangle: " + err);
        }
    });
}

function editTitle(file, title) {
    $.ajax({
        type: "get",
        dataType: "json",
        url: '/editTitle',
        data: {
            fileName: file,
            title: title
        },
        success: data => {
            if (data.sent == true) {
                window.location.reload();
            } else {
                console.log("Issue in editing the title.");
            }
        },
        fail: err => {
            console.log("Error occured in changing the title: " + err);
            alert("Error occured in changing the title: " + err);
        }
    });
}

function getNewTitle(file) {
    removeExtraChildrenViaPage();

    let root = document.getElementById('editTitleorDescription');
    root.style.border = "1px solid black";

    let header = document.createElement("h4");
    header.innerHTML = "Enter the new title.";
    root.appendChild(header);

    let value = document.createElement("input");
    value.type = "text";
    value.placeholder = "Enter new Title.";
    root.appendChild(value);
    let btn = document.createElement("input");
    btn.type = "button";
    btn.value = "Submit";
    btn.onclick = function () {
        removeExtraChildrenViaPage();
        editTitle(file, value.value);
    }
    root.appendChild(btn);
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
            titleCell.innerHTML = title.title + "\t";
            let btn = document.createElement("input");
            btn.type = "button";
            btn.value = "Edit";
            btn.onclick = function () {
                getNewTitle(file);
            }
            titleCell.append(btn);
        },
        
        fail: err => {
            console.log("Error occured in getting the title: " + err);
            alert("Error occured in getting the title: " + err);
        }
    });
}

function removeExtraChildrenViaPage() {
    let attr = document.getElementById('attributeTable');
    let editAttr = document.getElementById('editAttribute');
    let titOrDesc = document.getElementById('editTitleorDescription');
    let shape = document.getElementById('AddShape');
    while (attr.hasChildNodes()) {
        attr.removeChild(attr.firstChild);
    }
    attr.style.border = "none";
    while (editAttr.hasChildNodes()) {
        editAttr.removeChild(editAttr.firstChild);
    }
    editAttr.style.border = "none";
    while (titOrDesc.hasChildNodes()) {
        titOrDesc.removeChild(titOrDesc.firstChild);
    }
    titOrDesc.style.border = "none";
    while (shape.hasChildNodes()) {
        shape.removeChild(shape.firstChild);
    }
}

function getNewDescription(file) {
    removeExtraChildrenViaPage();

    let root = document.getElementById('editTitleorDescription');
    root.style.border = "1px solid black";

    let header = document.createElement("h4");
    header.innerHTML = "Enter the new description.";
    root.appendChild(header);

    let value = document.createElement("input");
    value.type = "text";
    value.placeholder = "Enter new Description.";
    root.appendChild(value);
    let btn = document.createElement("input");
    btn.type = "button";
    btn.value = "Submit";
    btn.onclick = function () {
        removeExtraChildrenViaPage();
        editDescription(file, value.value);
    }
    root.appendChild(btn);
}

function editDescription(file, desc) {
    $.ajax({
        type: "get",
        dataType: "json",
        url: '/editDescription',
        data: {
            fileName: file,
            description: desc
        },
        success: data => {
            if (data.sent == true) {
                window.location.reload();
            } else {
                console.log("Issue in editing the description.");
            }
        },
        fail: err => {
            console.log("Error occured in changing the description: " + err);
            alert("Error occured in changing the description: " + err);
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
            descCell.innerHTML = desc.desc + "\t";
            let btn = document.createElement("input");
            btn.type = "button";
            btn.value = "Edit";
            btn.onclick = function () {
                getNewDescription(file);
            }
            descCell.append(btn);
        },
        fail: err => {
            console.log("Error occured in getting the title: " + err);
            alert("Error occured in getting the title: " + err);
        }
    });
}

//List off all the rectangles, paths, circles, and groups that are in the svg file
async function addComponentDetails(file, root) {
    await $.ajax({
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
                let editAttr = document.createElement("input");
                editAttr.type = "button";
                editAttr.value = "Edit";
                editAttr.onclick = function () {
                    editAttributes(dataCur, file, "r");
                };
                attributes.appendChild(editAttr);

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

    await $.ajax({
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
                let editAttr = document.createElement("input");
                editAttr.type = "button";
                editAttr.value = "Edit";
                editAttr.onclick = function () {
                    editAttributes(dataCur, file, "c");
                };
                attributes.appendChild(editAttr);

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

    await $.ajax({
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
                let editAttr = document.createElement("input");
                editAttr.type = "button";
                editAttr.value = "Edit";
                editAttr.onclick = function () {
                    editAttributes(dataCur, file, "p");
                };
                attributes.appendChild(editAttr);

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

    await $.ajax({
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
                let editAttr = document.createElement("input");
                editAttr.type = "button";
                editAttr.value = "Edit";
                editAttr.onclick = function () {
                    editAttributes(dataCur, file, "g");
                };
                attributes.appendChild(editAttr);

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

    //row[n] => For adding shapes.
    addShapeButton(file, root);
}

//Display the list of all the attributes.
function showAttributes(JSONString, fileName, type) {
    let root = document.getElementById('editTitleorDescription');
    while (root.hasChildNodes()) {
        root.removeChild(root.firstChild);
    }
    root.style.border = "none";
    root = document.getElementById('editAttribute');
    while (root.hasChildNodes()) {
        root.removeChild(root.firstChild);
    }
    root.style.border = "none";
    root = document.getElementById('attributeTable');
    while (root.hasChildNodes()) {
        root.removeChild(root.firstChild);
    }
    let URL = '';
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

function editAttributes(JSONString, fileName, type) {
    let root = document.getElementById('editTitleorDescription');
    while (root.hasChildNodes()) {
        root.removeChild(root.firstChild);
    }
    root.style.border = "none";
    root = document.getElementById('attributeTable');
    while (root.hasChildNodes()) {
        root.removeChild(root.firstChild);
    }
    root = document.getElementById('editAttribute');
    while (root.hasChildNodes()) {
        root.removeChild(root.firstChild);
    }

    root.style.border = "1px solid black";
    let text = document.createElement("h4");
    text.innerHTML = "Enter the attribute name and value.";
    root.appendChild(text);

    let name = document.createElement("input");
    name.type = "text";
    name.placeholder = "Enter name of Attribute.";
    root.appendChild(name);
    let value = document.createElement("input");
    value.type = "text";
    value.placeholder = "Enter value of Attribute.";
    root.appendChild(value);
    let btn = document.createElement("input");
    btn.type = "button";
    btn.value = "Submit";
    btn.onclick = function () { 
        addAttribute({ name: name.value, value: value.value }, JSONString, fileName, type);
        while (root.hasChildNodes()) {
            root.removeChild(root.firstChild);
        }
        root.style.border = "none";
    }
    root.appendChild(btn);

    console.log("Called editAttributes function.");
}

function getIndex(JSONString, file, type) {
    let URL = 1;
    if (type == "r") {
        URL = '/getRects';
    } else if (type == "c") {
        URL = '/getCircs';
    } else if (type == "p") {
        URL = '/getPaths';
    } else if (type == "g") {
        URL = '/getGroups'
    }
    $.ajax({
        type: "get",
        dataType: "json",
        url: URL,
        data: {
            fileName: file
        },
        success: data => {
            let rects = JSON.parse(data);
            let i = 0;
            rects.forEach(elem => {
                if (JSON.stringify(elem) === JSONString) {
                    return i;
                }
                i++;
            });
            return -1;
        },
        fail: err => {
            console.log("Error occured: " + err);
            alert("Error occured: " + err);
        }
    });
}

function addAttribute(data, JSONString, fileName, type) {
    let index = getIndex(JSONString, fileName, type);
    console.log("adding attribute...\n");
    console.log(data);
     $.ajax({
        type: "get",
        dataType: "json",
        url: '/addAttribute',
        data: {
            index: index,
            data: data,
            fileName: fileName,
            type: type
        },
         success: data => {
             console.log(data);
             if (data.sent == true) {
                 window.location.reload();
                //  displayIndividualDetails(fileName);
             } else {
                 console.log("issue in adding that atttribute.\n");
            }
        },
        fail: err => {
            console.log("Error occured in adding attributes: " + err);
            alert("Error occured in adding attributes: " + err);
        }
    });
}

