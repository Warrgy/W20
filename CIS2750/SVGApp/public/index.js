
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
            /*  Do something with returned object
                Note that what we get is an object, not a string, 
                so we do not need to parse it on the server.
                JavaScript really does handle JSONs seamlessly
            */
            // $('#blah').html("On page load, received string '"+data.foo+"' from server");
            //We write the object to the console to show that the request was successful
            $('#rectangles').html(data.numRect);
            $('#circles').html(data.numCirc);
            $('#paths').html(data.numPaths);
            $('#groups').html(data.numGroups);
            console.log("Successfully displayed: " + JSON.stringify(data)); 
            

            // fetch('/getAllFiles')
            //     .then(
            //         function(response) {
            //         if (response.status !== 200) {
            //             console.log('Looks like there was a problem. Status Code: ' +
            //             response.status);
            //             return;
            //         }

            //         // Examine the text in the response
            //         response.json().then(function(data) {
            //             data.forEach(element => {
            //                 createRow(element, )
            //             });
            //             console.log(data);
            //         });
            //         }
            //     )
            //     .catch(function(err) {
            //         console.log('Fetch Error :-S', err);
            //     });

            // console.log("inside num funct: files = " + allFiles);

            // console.log("files:\n");
            // console.log(files);
        },
        fail: function(error) {
            // Non-200 return, do something with error
            // $('#blah').html("On page load, received error from server");
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
        }
    });

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
        fail: function(error) {
            console.log("Received error when getting SVGtoJSON: " + error);
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
            let stringSize = data.size / 1000;
            stringSize += "KB";
            cell.innerHTML = stringSize;
        },
        fail: err => {
            console.log("Error occured in getting the file size: " + err);
        }
    });
}