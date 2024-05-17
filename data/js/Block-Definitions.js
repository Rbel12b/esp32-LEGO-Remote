var NUM_BLOCKS = 19;
function getBlockHtml(index = 0, Code = [new BlockData()]) {
    switch (Code[index].code) {
        case 1:
            return '<div class="MoveBlock">Turn Motor by </div>\
            <input type="number" class="argInput" id="' + index + '_Arg" value="' + Code[index].args[0] + '" onchange="ArgChange(this)" name="0"> \
            degrees, Hub: \
            <input type="number" class="argInput" id="' + index + '_Arg" value="' + Code[index].args[1] + '" onchange="ArgChange(this)" name="1" min="0" max="7">, \
            Port: \
            <input type="number" class="argInput" id="' + index + '_Arg" value="' + Code[index].args[2] + '" onchange="ArgChange(this)" name="2" min="0" max="5">';
        case 2:
            return '<div class="MoveBlock">Set Motor speed to </div>\
            <input type="number" class="argInput" id="' + index + '_Arg" value="' + Code[index].args[0] + '" onchange="ArgChange(this)" name="0">\
            %, HUB: \
            <input type="number" class="argInput" id="' + index + '_Arg" value="' + Code[index].args[1] + '" onchange="ArgChange(this)" name="1" min="0" max="7">\
            , Port: \
            <input type="number" class="argInput" id="' + index + '_Arg" value="' + Code[index].args[2] + '" onchange="ArgChange(this)" name="2" min="0" max="5">';
        case 3:
            return '<div class="MoveBlock">Run Motor for </div>\
            <input type="number" class="argInput" id="' + index + '_Arg" value="' + Code[index].args[0] + '" onchange="ArgChange(this)" name="0">\
            milliseconds, HUB: \
            <input type="number" class="argInput" id="' + index + '_Arg" value="' + Code[index].args[1] + '" onchange="ArgChange(this)" name="1" min="0" max="7">\
            , Port: \
            <input type="number" class="argInput" id="' + index + '_Arg" value="' + Code[index].args[2] + '" onchange="ArgChange(this)" name="2" min="0" max="5">';
        case 4:
            return '<div class="MoveBlock">Turn Motor to absolute posision </div>\
            <input type="number" class="argInput" id="' + index + '_Arg" value="' + Code[index].args[0] + '" onchange="ArgChange(this)" name="0">\
            °, HUB: \
            <input type="number" class="argInput" id="' + index + '_Arg" value="' + Code[index].args[1] + '" onchange="ArgChange(this)" name="1" min="0" max="7">\
            , Port: \
            <input type="number" class="argInput" id="' + index + '_Arg" value="' + Code[index].args[2] + '" onchange="ArgChange(this)" name="2" min="0" max="5">';
        case 5:
            return '<div class="MoveBlock">Set Motor encoder to </div>\
            <input type="number" class="argInput" id="' + index + '_Arg" value="' + Code[index].args[0] + '" onchange="ArgChange(this)" name="0">\
            °, HUB: \
            <input type="number" class="argInput" id="' + index + '_Arg" value="' + Code[index].args[1] + '" onchange="ArgChange(this)" name="1" min="0" max="7">\
            , Port: \
            <input type="number" class="argInput" id="' + index + '_Arg" value="' + Code[index].args[2] + '" onchange="ArgChange(this)" name="2" min="0" max="5">';
        case 7:
            return '<div class="MoveBlock">var </div>\
            <input type="number" class="argInput" id="' + index + '_Arg" value="' + Code[index].args[0] + '" onchange="ArgChange(this)" name="0" min="0" max="255">\
            equals to \
            <input type="number" class="argInput" id="' + index + '_Arg" value="' + Code[index].args[1] + '" onchange="ArgChange(this)" name="1" min="0" max="255">';
        case 8:
            return '<div class="MoveBlock">var </div>\
            <input type="number" class="argInput" id="' + index + '_Arg" value="' + Code[index].args[0] + '" onchange="ArgChange(this)" name="0" min="0" max="255">\
            * var \
            <input type="number" class="argInput" id="' + index + '_Arg" value="' + Code[index].args[1] + '" onchange="ArgChange(this)" name="1" min="0" max="255">';
        case 9:
            return '<div class="MoveBlock">var </div>\
            <input type="number" class="argInput" id="' + index + '_Arg" value="' + Code[index].args[0] + '" onchange="ArgChange(this)" name="0" min="0" max="255">\
            / var \
            <input type="number" class="argInput" id="' + index + '_Arg" value="' + Code[index].args[1] + '" onchange="ArgChange(this)" name="1" min="0" max="255">';
        case 10:
            return '<div class="MoveBlock">var </div>\
            <input type="number" class="argInput" id="' + index + '_Arg" value="' + Code[index].args[0] + '" onchange="ArgChange(this)" name="0" min="0" max="255">\
            + var \
            <input type="number" class="argInput" id="' + index + '_Arg" value="' + Code[index].args[1] + '" onchange="ArgChange(this)" name="1" min="0" max="255">';
        case 11:
            return '<div class="MoveBlock">var </div>\
            <input type="number" class="argInput" id="' + index + '_Arg" value="' + Code[index].args[0] + '" onchange="ArgChange(this)" name="0" min="0" max="255">\
            - var \
            <input type="number" class="argInput" id="' + index + '_Arg" value="' + Code[index].args[1] + '" onchange="ArgChange(this)" name="1" min="0" max="255">';
        case 12:
            return '<div class="MoveBlock">If var </div>\
            <input type="number" class="argInput" id="' + index + '_Arg" value="' + Code[index].args[0] + '" onchange="ArgChange(this)" name="0" min="0" max="255">';
        case 13:
            return '<div class="MoveBlock">var </div>\
            <input type="number" class="argInput" id="' + index + '_Arg" value="' + Code[index].args[0] + '" onchange="ArgChange(this)" name="0" min="0" max="255">\
            <div class="custom-select" name="bool"><select id="bool_' + index + '" value="' + Code[index].args[1] + '" onchange="selectArgChange(this)" name="1">\
                <option value="0">equals</option>\
                <option value="1">greater than</option>\
                <option value="2">less than</option>\
                <option value="3">or</option>\
                <option value="4">and</option>\
                <option value="5">not</option>\
            </select></div> \
            var \
            <input type="number" class="argInput" id="' + index + '_Arg" value="' + Code[index].args[2] + '" onchange="ArgChange(this)" name="2" min="0" max="255">';
        case 14:
            return '<span class="custom-select" name="math"><select id="math_' + index + '" value="' + Code[index].args[0] + '" onchange="selectArgChange(this)" name="0">\
                <option value="0">round</option>\
                <option value="1">abs</option>\
                <option value="2">sqrt</option>\
                <option value="3">sin</option>\
                <option value="4">cos</option>\
                <option value="5">tan</option>\
                <option value="6">log</option>\
                <option value="7">ln</option>\
                <option value="8">e^</option>\
                <option value="9">10^</option>\
                <option value="10">asin</option>\
                <option value="11">acos</option>\
                <option value="12">atan</option>\
                <option value="13">floor</option>\
                <option value="14">ceiling</option>\
            </select></span>\
            <div class="MoveBlock">(var </div>\
            <input type="number" class="argInput" id="' + index + '_Arg" value="' + Code[index].args[1] + '" onchange="ArgChange(this)" name="1" min="0" max="255">\
            )';
        case 15:
            return '<div class="MoveBlock">var</div>\
            <input type="number" class="argInput" id="' + index + '_Arg" value="' + Code[index].args[0] + '" onchange="ArgChange(this)" name="0" min="0" max="255">\
            Mod var \
            <input type="number" class="argInput" id="' + index + '_Arg" value="' + Code[index].args[1] + '" onchange="ArgChange(this)" name="1" min="0" max="255">';
        case 16:
            return '<div class="MoveBlock">Set Motor PWM to </div>\
            <input type="number" class="argInput" id="' + index + '_Arg" value="' + Code[index].args[0] + '" onchange="ArgChange(this)" name="0">\
            , Channel: \
            <input type="number" class="argInput" id="' + index + '_Arg" value="' + Code[index].args[1] + '" onchange="ArgChange(this)" name="1" min="0" max="3">\
            , Port: \
            <input type="number" class="argInput" id="' + index + '_Arg" value="' + Code[index].args[2] + '" onchange="ArgChange(this)" name="2" min="0" max="1">';
        case 17:
            return '<div class="MoveBlock">Increment, Channel: </div>\
            <input type="number" class="argInput" id="' + index + '_Arg" value="' + Code[index].args[0] + '" onchange="ArgChange(this)" name="0" min="0" max="3">\
            , Port: \
            <input type="number" class="argInput" id="' + index + '_Arg" value="' + Code[index].args[1] + '" onchange="ArgChange(this)" name="1" min="0" max="1">';
        case 18:
            return '<div class="MoveBlock">Decrement, Channel: </div>\
            <input type="number" class="argInput" id="' + index + '_Arg" value="' + Code[index].args[0] + '" onchange="ArgChange(this)" name="0" min="0" max="3">\
            , Port: \
            <input type="number" class="argInput" id="' + index + '_Arg" value="' + Code[index].args[1] + '" onchange="ArgChange(this)" name="1" min="0" max="1">';
        case 19:
            return '<div class="MoveBlock">var </div>\
            <input type="number" class="argInput" id="' + index + '_Arg" value="' + Code[index].args[0] + '" onchange="ArgChange(this)" name="0" min="0" min="0" max="255">';
        default:
            return "Unknown"
    }
}
const BLOCK_NAME = [
    "Turn Motor by 0° degrees, HUB: 0, Port: 0",
    "Set Motor speed to 0%, HUB: 0, Port: 0",
    "Run Motor for 0 milliseconds, HUB: 0, Port: 0",
    "Turn Motor to absolute posision 0°, HUB: 0, Port: 0",
    "Set Motor encoder to 0°, HUB: 0, Port: 0",
    "",
    "var 0 equals to 0",
    "var 0 * var 0",
    "var 0 / var 0",
    "var 0 + var 0",
    "var 0 - var 0",
    "If var 0",
    "var 0 equals? var 0",
    "round(var 0)",
    "var0 Mod var 0",
    "Set Motor PWM to 0, Channel: 0, Port: 0",
    "Increment, Channel: 0, Port: 0",
    "Decrement, Channel: 0, Port: 0",
    "var 0",
]
const BLOCK_COLOR = [
    "", // not a valid index
    "#0080ff",
    "#0080ff",
    "#0080ff",
    "#0080ff",
    "#0080ff",
    "",
    "#ffa500",
    "#00a560",
    "#00a560",
    "#00a560",
    "#00a560",
    "#ffff00",
    "#00ffff",
    "#00a560",
    "#00a560",
    "#0080ff",
    "#0080ff",
    "#0080ff",
    "#ffa500",
]
const BLOCK_TEXT_COLOR = [
    "", // not a valid index
    "#ffffff",
    "#ffffff",
    "#ffffff",
    "#ffffff",
    "#ffffff",
    "",
    "#ffffff",
    "#ffffff",
    "#ffffff",
    "#ffffff",
    "#ffffff",
    "#000000",
    "#ffffff",
    "#ffffff",
    "#ffffff",
    "#ffffff",
    "#ffffff",
    "#ffffff",
    "#ffffff",
]
const BLOCK_LEN = [
    0, // not a valid index
    3,
    3,
    3,
    3,
    3,
    0,
    3,
    3,
    3,
    3,
    3,
    1,
    4,
    3,
    3,
    3,
    2,
    2,
    1,
]