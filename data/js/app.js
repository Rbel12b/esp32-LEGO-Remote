function submitCode() {
    const xhr = new XMLHttpRequest();
    xhr.open("POST", "/editor");
    xhr.onload = () => {
        if (xhr.readyState == 4 && xhr.status == 200) {
        } else {
            console.log(`Error: ${xhr.status}`);
        }
    };
    xhr.send(CopileCode(Code));
}
function loadCode() {
    const xhr = new XMLHttpRequest();
    xhr.open("GET", "/editor");
    xhr.onload = () => {
        if (xhr.readyState == 4 && xhr.status == 201) {
            Code = GetCode(xhr.response);
            CodeLenght = GetCodelLength(xhr.response);
        } else {
            console.log(`Error: ${xhr.status}`);
        }
    };
    xhr.send();
}
function setup() {
    loadCode();
    SetupBlocks();
    Code.pop();
    document.getElementById("Code").innerHTML = "";
}
const Code = [0];
var CodeLenght = 0;

function SetupBlocks() {
    var html = "";
    for (var i = 0; i < NUM_BLOCKS; i++) {
        if(BLOCK_NAME[i] == ""){
            continue;
        }
        html += ("<button name=\"" + (i + 1) + "\" class=\"CodeBlock ListBlock\" onclick=\"AddBlock(this)\">" + BLOCK_NAME[i] + "</button><br>");
    }
    document.getElementById("BlockList").innerHTML = html;
    for (var i = 0; i < NUM_BLOCKS; i++) {
        if(BLOCK_NAME[i] == ""){
            continue;
        }
        document.getElementsByName(i + 1)[0].style.backgroundColor = BLOCK_COLOR[i + 1];
        document.getElementsByName(i + 1)[0].style.color = BLOCK_TEXT_COLOR[i + 1];
    }
}
function AddBlock(block) {
    var newBlock = block.name;
    var blockNum = Number(newBlock)
    Code.push(blockNum);
    for (let i = 0; i < BLOCK_LEN[blockNum]; i++) {
        Code.push(0);
        CodeLenght += 1;
    }
    CodeLenght += 1;
    updateCode();
}
function updateCode() {
    var html = "";
    for (let i = 0; i < CodeLenght; i++) {
        let len = BLOCK_LEN[Code[i]]
        i += len;
        if (i == CodeLenght - 1) {
            html += ("<div  class=\"CodeBlock\" id=\"" + (i - len) + "_Block\">" + getBlockHtml(i - len, Code) + "</div><br>");
        }
    }
    document.getElementById("Code").innerHTML += html;
    for (let i = 0; i < CodeLenght; i++) {
        let element = document.getElementById(i + "_Block");
        dragElement(element);
        let len = BLOCK_LEN[Code[i]]
        i += len;
        if (i == CodeLenght - 1) {
            element.style.backgroundColor = BLOCK_COLOR[Code[(i - len)]]
            dragElement(element);
        }
    }
}
function ArgChange(BlockArg) {
    let val = BlockArg.value;
    let arg = BlockArg.id.slice(0, -4); // sclie off "_Arg" of the end
    Code[arg] = Number(val);
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
        document.getElementById("Code").onmousemove = elementDrag;
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
        document.getElementById("Code").onmousemove = null;
    }
}