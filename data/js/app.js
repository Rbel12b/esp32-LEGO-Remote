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
        if (BLOCK_NAME[i] == "") {
            continue;
        }
        html += ("<button name=\"" + (i + 1) + "\" class=\"CodeBlock ListBlock\" onclick=\"AddBlock(this)\">" + BLOCK_NAME[i] + "</button>");
    }
    document.getElementById("BlockList").innerHTML = html;
    for (var i = 0; i < NUM_BLOCKS; i++) {
        if (BLOCK_NAME[i] == "") {
            continue;
        }
        document.getElementsByName(i + 1)[0].style.backgroundColor = BLOCK_COLOR[i + 1];
        document.getElementsByName(i + 1)[0].style.color = BLOCK_TEXT_COLOR[i + 1];
    }
}
const BlockPos = [{ top: 0, left: 0 }];
var NumbeOfBlocks = 0;
function AddBlock(block) {
    var newBlock = block.name;
    var blockNum = Number(newBlock)
    Code.push(blockNum);
    var pos = { top: 0, left: 0 };
    pos.top = 0;
    pos.left = 0;
    BlockPos.push(pos);
    for (var i = 0; i < BLOCK_LEN[blockNum]; i++) {
        Code.push(0);
        CodeLenght += 1;
    }
    CodeLenght += 1;
    NumbeOfBlocks++;
    updateCode();
}
function deleteBlock(Block) {
    if (Block.attachedBlock) {
        deleteBlock(Block.attachedBlock);
    }
    var index = Number(Block.id.slice(0, -6));
    var Bnumber = Number(Block.name);
    var len = BLOCK_LEN[Code[index]];
    Code.splice(index, len + 1);
    CodeLenght -= (1 + len);
    BlockPos.splice(Bnumber, 1);
    NumbeOfBlocks--;
    Block.remove();
    var num = 0;
    for (var i = index; i < CodeLenght; i++) {
        document.getElementById((i + len + 1) + "_Block").name = (num + "_Block");
        document.getElementById((i + len + 1) + "_Block").id = (i + "_Block");
        i += BLOCK_LEN[Code[i]];
        num++;
    }
    scanBlockPos();
}
function updateCode() {
    var html = "";
    var num = 0;
    for (var i = 0; i < CodeLenght; i++) {
        var len = BLOCK_LEN[Code[i]];
        i += len;
        if (i == CodeLenght - 1) {
            html += ("<div  class=\"CodeBlock\" id=\"" + (i - len) + "_Block\" name=\"" + num + "_Block\">" + getBlockHtml(i - len, Code) + "\
            <div class=\"CodeBlock BlockConnector\"></div></div>");
        }
        num++;
    }
    document.getElementById("Code").innerHTML += html;
    var offset = 0;
    for (var i = 0; i < CodeLenght; i++) {
        var element = document.getElementById(i + "_Block");
        while (searchPos(offset - 75, offset - 25, offset - 75, offset - 25, element) == false && offset > 0) {
            offset -= 50;
        }
        dragElement(element);
        var len = BLOCK_LEN[Code[i]];
        i += len;
        if (i == CodeLenght - 1) {
            element.style.backgroundColor = BLOCK_COLOR[Code[(i - len)]];
            element.getElementsByClassName("BlockConnector")[0].style.backgroundColor = BLOCK_COLOR[Code[(i - len)]];
            element.style.color = BLOCK_TEXT_COLOR[Code[(i - len)]];
            element.style.left = offset + "px";
            element.style.top = offset + "px";
        }
        offset += 50;
    }
    scanBlockPos();
}
function scanBlockPos() {
    var num = 0;
    for (var i = 0; i < CodeLenght; i++) {
        var element = document.getElementById(i + "_Block");
        BlockPos[num].top = element.style.top.slice(0, -2);
        BlockPos[num].left = element.style.left.slice(0, -2);
        var len = BLOCK_LEN[Code[i]];
        i += len;
        num++;
    }
}
function searchPos(leftMin, leftMax, topMin, topMax, block) {
    for (var i = 0; i < NumbeOfBlocks; i++) {
        var pos = BlockPos[i];
        if (pos.left >= leftMin &&
            pos.left <= leftMax &&
            pos.top >= topMin &&
            pos.top <= topMax) {
            var r = document.getElementsByName(i + "_Block")[0];
            if(r != block){
                return r;
            }
        }
    }
    return false;
}
function ArgChange(BlockArg) {
    var val = BlockArg.value;
    var arg = BlockArg.id.slice(0, -4); // sclie off "_Arg" of the end
    Code[arg] = Number(val);
    return true;
}
function selectArgChange(BlockArg) {
    var val = BlockArg.value;
    var arg = BlockArg.id.slice(5, 10); // sclie off "math_" of the start
    Code[arg] = Number(val);
    return true;
}
function dragElement(elmnt) {
    var pos1 = 0, pos2 = 0, pos3 = 0, pos4 = 0;
    var block = document.getElementById("Code");

    /* otherwise, move the DIV from anywhere inside the DIV:*/
    elmnt.getElementsByTagName("div")[0].onmousedown = dragMouseDown;

    function dragMouseDown(e) {
        //e = e || window.event;
        e.preventDefault();
        // get the mouse cursor position at startup:
        var left = elmnt.style.left;
        left = left.slice(0, -2);
        var top = elmnt.style.top;
        top = top.slice(0, -2);
        pos1 = e.clientX - left;
        pos2 = e.clientY - top;
        document.onmouseup = closeDragElement;
        // call a function whenever the cursor moves:
        document.onmousemove = elementDrag;
        if(elmnt.attachedToBlock){
            elmnt.attachedToBlock.attachedBlock = undefined;
            elmnt.attachedToBlock = undefined;
        }
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
        var left = elmnt.style.left;
        left = left.slice(0, -2);
        var top = elmnt.style.top;
        top = top.slice(0, -2);
        if (elmnt.attachedBlock) {
            Drag(elmnt.attachedBlock, (pos4 - pos2), (pos3 - pos1));
        }
        var availablebBlock = searchPos(left - 50, left + 50, top - 125, top, elmnt);
        if(block != availablebBlock && block){
            block.getElementsByClassName("BlockConnector")[0].style.borderBottomColor = "#808080";
        }
        if (availablebBlock) {
            availablebBlock.getElementsByClassName("BlockConnector")[0].style.borderBottomColor = "#ffffff";
            block = availablebBlock;
        }
    }

    function closeDragElement() {
        /* stop moving when mouse button is released:*/
        document.onmouseup = null;
        document.onmousemove = null;
        var left = elmnt.style.left;
        left = left.slice(0, -2);
        var top = elmnt.style.top;
        top = top.slice(0, -2);
        if (left < -180) {
            deleteBlock(elmnt);
            scanBlockPos();
            return;
        }
        block = searchPos(left - 50, left + 50, top - 125, top, elmnt);
        if (block) {
            block.getElementsByClassName("BlockConnector")[0].style.borderBottomColor = "#808080"
            Drag(elmnt,Number(block.style.top.slice(0,-2)),Number(block.style.left.slice(0,-2)));
            if (block.attachedBlock) {
                var endelement = elmnt.attachedBlock;
                while (endelement.attachedBlock) {
                    endelement = endelement.attachedBlock;
                }
                endelement.attachedBlock = block.attachedBlock;
            }
            block.attachedBlock = elmnt;
            elmnt.attachedToBlock = block;
        }
        scanBlockPos();
    }
}
function Drag(element, _top = 0, _left = 0) {
    element.style.top = (_top + 51) + "px";
    element.style.left = (_left) + "px";
    if (element.attachedBlock) {
        Drag(element.attachedBlock, _top + 51, _left);
    }
}