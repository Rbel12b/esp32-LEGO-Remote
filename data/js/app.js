function submitCode() {
    const xhr = new XMLHttpRequest();
    xhr.open("POST", "/editor");
    xhr.onload = () => {
        if (xhr.readyState == 4 && xhr.status == 200) {
        } else {
            console.log(`Error: ${xhr.status}`);
        }
    };
    xhr.send(Code);
}
function loadCode() {
    const xhr = new XMLHttpRequest();
    xhr.open("GET", "/editor");
    xhr.onload = () => {
        if (xhr.readyState == 4 && xhr.status == 201) {
            Code = xhr.response;
        } else {
            console.log(`Error: ${xhr.status}`);
        }
    };
    xhr.send();
}
function setup() {
    loadCode();
    SetupBlocks();
    dragElement(document.getElementById("Block"));
}
var Code = "";
var NUM_BLOCKS = 2;
const BLOCK_NAME = [
    "Motor Speed: 0",
    "Turn motor 0°"
]
const BLOCK_LEN = [
    3,
    3
]
function SetupBlocks() {
    var html = "";
    for (var i = 0; i < NUM_BLOCKS; i++) {
        html += ("<button name=\"" + (i + 1) + "\" class=\"Block_List\" onclick=\"AddBlock(this)\">" + BLOCK_NAME[i] + "</button><br>");
    }
    document.getElementById("BlockList").innerHTML = html;
    for (var i = 0; i < NUM_BLOCKS; i++) {
        //Make the DIV element draggagle:
        //dragElement(document.getElementsByName("" + (i + 1))[0]);
    }
}
function AddBlock(block) {
    var newBlock = block.name;
    var blockNum = Number(newBlock)
    Code += blockNum;
    for (var i = 0; i < BLOCK_LEN[blockNum]; i++) {
        Code += '\0';
    }
    console.log(blockNum);
    updateCode();
}
function updateCode() {

}

function dragElement(elmnt) {
    var pos1 = 0, pos2 = 0, pos3 = 0, pos4 = 0;

    /* otherwise, move the DIV from anywhere inside the DIV:*/
    elmnt.onmousedown = dragMouseDown;

    function dragMouseDown(e) {
        //e = e || window.event;
        e.preventDefault();
        // get the mouse cursor position at startup:
        let left = elmnt.style.left;
        left = left.slice(0, -2);
        let top = elmnt.style.top;
        top = top.slice(0, -2);
        pos1 = e.clientX - left;
        pos2 = e.clientY - top;
        document.onmouseup = closeDragElement;
        // call a function whenever the cursor moves:
        document.onmousemove = elementDrag;
    }

    function elementDrag(e) {
        //e = e || window.event;
        e.preventDefault();
        // calculate the new cursor position:
        pos3 = e.clientX;
        pos4 = e.clientY;
        // set the element's new position:
        elmnt.style.top = (pos4 - pos2) + "px";
        elmnt.style.left = (pos3 - pos1) + "px";
    }

    function closeDragElement() {
        /* stop moving when mouse button is released:*/
        document.onmouseup = null;
        document.onmousemove = null;
    }
}