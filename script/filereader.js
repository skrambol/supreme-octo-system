'use strict';

var filereader = {
    init: function(div) {
        if (window.File && window.FileReader && window.FileList && window.Blob) {

            $(div).prepend([
                '<div id="file-reading" class="col s12">',
                    '<div class="input-field col s12">',
                        '<input id="file" type="file">',
                    '</div>',
                '</div>',
            ].join(''));

            $('#file')[0].addEventListener('change', this.readFiles, false);
        }

        else {
            alert('File reading is not available in this browser');
        }
    },

    readFiles: function(event) {
        var files = event.target.files,
            file = files[0],
            reader = new FileReader(),
            boards = [],
            puzzle_count=0,
            puzzle_no=0,
            dims,
            input,
            i=1;

        reader.onload = function(e) {
            input = reader.result.split(/[\r\n]+/g);
            puzzle_count = input[0];

            for (puzzle_no=0; puzzle_no<puzzle_count; puzzle_no++) {
                dims = input[i++];
                boards.push([]);

                for (var j=0; j<dims; j++) {
                        boards[puzzle_no][j] = input[i++].split(' ');
                }
            }

            $('#solve-btn').removeClass('disabled');
            chancellor.createPagination(puzzle_count, function(index) {
                chancellor.createTable('#board', boards[index-1].length, false, boards[index-1]);
                chancellor.solveTable();
            }, '#puzzles-pagination');
        }
        reader.readAsText(file)
    }
}
