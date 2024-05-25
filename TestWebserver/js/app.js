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
            NumbeOfBlocks = GetCodelLength(xhr.response);
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
class BlockData {
    constructor(previous = undefined, next = undefined, code = 0, args = [0], block = HTMLElement, id = 0) {
        this.prev = previous;
        this.next = next;
        this.code = code;
        this.args = args;
        this.block = block;
        this.id = id;
        var pos = { top: 0, left: 0 };
        pos.top = 0;
        pos.left = 0;
        this.pos = pos;
        this.pos1 = 0;
        this.pos2 = 0;
        this.pos3 = 0;
        this.pos4 = 0;
        return this;
    }
}
const Code = [new BlockData()];

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
function AddBlock(block = HTMLElement) {
    var newBlock = block.name;
    var blockNum = Number(newBlock)
    Code.push(new BlockData(undefined, undefined, blockNum, [], undefined, NumbeOfBlocks));
    for (var i = 0; i < BLOCK_LEN[blockNum]; i++) {
        Code[NumbeOfBlocks].args.push(0);
    }
    NumbeOfBlocks++;
    updateCode();
}
function deleteBlock(Block = new BlockData()) {
    do {
        var index = Block.id;
        NumbeOfBlocks--;
        Block.block.remove();
        Block = Block.next;
        Code.splice(index, 1);
        BlockPos.splice(index, 1);
        for (var i = 0; i < NumbeOfBlocks; i++) {
            Code[i].block.id = (i + "_Block");
            Code[i].id = i;
        }
    } while (Block);
}
function updateCode() {
    var i = NumbeOfBlocks - 1;
    var newBlock = document.createElement("div");
    newBlock.innerHTML = getBlockHtml(i, Code) + '<div class="CodeBlock BlockConnector"></div>';
    newBlock.id = i + '_Block';
    newBlock.className = 'CodeBlock';
    document.getElementById("Code").appendChild(newBlock);
    Code[i].block = document.getElementById(i + "_Block");

    var offset = 0;
    for (i = 0; i < NumbeOfBlocks; i++) {
        var element = Code[i].block;
        while (searchPos(offset - 75, offset - 25, offset - 75, offset - 25, Code[i]) == false && offset > 0) {
            offset -= 50;
        }
        if (i == NumbeOfBlocks - 1) {
            element.style.backgroundColor = BLOCK_COLOR[Code[i].code];
            element.getElementsByClassName("BlockConnector")[0].style.backgroundColor = BLOCK_COLOR[Code[i].code];
            element.style.color = BLOCK_TEXT_COLOR[Code[i].code];
            element.style.left = offset + "px";
            element.style.top = offset + "px";
            Code[i].pos.left = offset;
            Code[i].pos.top = offset;
        }
        offset += 50;
        element.getElementsByTagName("div")[0].removeEventListener("mousedown", dragBlock);
        element.getElementsByTagName("div")[0].addEventListener("mousedown", dragBlock);
    }
}
function searchPos(leftMin, leftMax, topMin, topMax, block) {
    for (var i = 0; i < NumbeOfBlocks; i++) {
        var pos = Code[i].pos;
        if (pos.left >= leftMin &&
            pos.left <= leftMax &&
            pos.top >= topMin &&
            pos.top <= topMax) {
            var r = Code[i];
            if (r != block) {
                return r;
            }
        }
    }
    return false;
}
function ArgChange(BlockArg = HTMLElement) {
    var val = BlockArg.value;
    var arg = BlockArg.id.slice(0, -4); // sclie off "_Arg" of the end
    var num = Number(BlockArg.name);
    Code[arg].args[num] = Number(val);
    return true;
}
function selectArgChange(BlockArg) {
    var val = BlockArg.value;
    var arg = BlockArg.id.slice(5, 10); // sclie off "math_" of the start
    var num = Number(BlockArg.name);
    Code[arg].args[num] = Number(val);
    return true;
}
function dragBlock(e) {
    if(!e.target.parentElement){
        return;
    }
    var elmnt = Code[Number(e.target.parentElement.id.slice(0, -6))];
    if(!elmnt){
        return;
    }
    //console.log(elmnt);
    e = e || window.event;
    e.preventDefault();
    // get the mouse cursor position at startup:
    var left = elmnt.block.style.left;
    left = left.slice(0, -2);
    var top = elmnt.block.style.top;
    top = top.slice(0, -2);
    elmnt.pos1 = e.clientX - left;
    elmnt.pos2 = e.clientY - top;
    document.addEventListener("mouseup", closeDragElement);
    // call a function whenever the cursor moves:
    document.addEventListener("mousemove", elementDrag);
    if (elmnt.prev) {
        elmnt.prev.next = undefined;
        elmnt.prev = undefined;
    }
    return elmnt;
}
function elementDrag(e) {
    if(!e.target.parentElement){
        return;
    }
    var elmnt = Code[Number(e.target.parentElement.id.slice(0, -6))];
    if(!elmnt){
        return;
    }
    var block = elmnt;
    //e = e || window.event;
    e.preventDefault();
    // calculate the new cursor position:
    elmnt.pos3 = e.clientX;
    elmnt.pos4 = e.clientY;
    // set the element's new position:
    elmnt.block.style.top = (elmnt.pos4 - elmnt.pos2) + "px";
    elmnt.block.style.left = (elmnt.pos3 - elmnt.pos1) + "px";
    var left = elmnt.block.style.left;
    left = left.slice(0, -2);
    var top = elmnt.block.style.top;
    top = top.slice(0, -2);
    elmnt.pos.left = left;
    elmnt.pos.top = top;
    if (elmnt.next) {
        Drag(elmnt.next, (elmnt.pos4 - elmnt.pos2), (elmnt.pos3 - elmnt.pos1));
    }
    var availablebBlock = searchPos(left - 75, left + 75, top - 125, top, elmnt);
    if (block != availablebBlock && block) {
        block.block.getElementsByClassName("BlockConnector")[0].style.borderBottomColor = "#808080";
    }
    if (availablebBlock) {
        availablebBlock.block.getElementsByClassName("BlockConnector")[0].style.borderBottomColor = "#ffffff";
        block = availablebBlock;
    }
}

function closeDragElement(e) {
    if(!e.target.parentElement){
        return;
    }
    var elmnt = Code[Number(e.target.parentElement.id.slice(0, -6))];
    if(!elmnt){
        return;
    }
    var block = elmnt;
    /* stop moving when mouse button is released:*/
    document.removeEventListener("mouseup", closeDragElement);
    document.removeEventListener("mousemove", elementDrag);
    var left = elmnt.block.style.left;
    left = left.slice(0, -2);
    var top = elmnt.block.style.top;
    top = top.slice(0, -2);
    if (left < -180) {
        deleteBlock(elmnt);
        return;
    }
    block = searchPos(left - 75, left + 75, top - 125, top, elmnt);
    if (block) {
        block.block.getElementsByClassName("BlockConnector")[0].style.borderBottomColor = "#808080"
        if (block.next) {
            var endelement = elmnt.next;
            while (endelement && endelement.next) {
                endelement = endelement.next;
            }
            if (endelement) {
                endelement.next = block.next;
                block.prev = endelement;
            }
            else {
                endelement.next = block.next;
                block.prev = endelement;
            }
        }
        block.next = elmnt;
        elmnt.prev = block;
        Drag(elmnt, Number(block.block.style.top.slice(0, -2)), Number(block.block.style.left.slice(0, -2)));
    }
}
function Drag(element = new BlockData(), _top = 0, _left = 0) {
    element.block.style.top = (_top + 55) + "px";
    element.block.style.left = (_left) + "px";
    element.pos.left = _left;
    element.pos.top = _top + 55;
    while (element.next != undefined) {
        _top += 55;
        element = element.next;
        element.style.top = (_top + 55) + "px";
        element.style.left = (_left) + "px";
        element.pos.left = _left;
        element.pos.top = _top + 55;
    }
}