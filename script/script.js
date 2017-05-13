'use strict';

$(document).ready( function () {
	chancellor.init();
});

var chancellor = {
	board : [],

	count : 0,

	win_flag : false,

	init : function(){

		$('select').material_select();

		$('#dim-specify')
			.keypress(function (e) {
				if (e.keyCode == 13) {
					var dim = Number($('#dim-specify').val()); 
					if(!dim || dim < 0) return Materialize.toast("Please enter a valid dimension number.", 2500);
					chancellor.createTable('#board',Number($('#dim-specify').val()));
					$('#solve-btn').removeClass('disabled');
				}
			});

		$('#reset-btn')
			.click(function(){
				chancellor.createTable('#board', chancellor.board.length);				
			});

		$('#solve-btn')
			.click(function(){
				chancellor.solveTable();				
			});

		$('body').on('click', '.tile', function(){
			var x = Number($(this).attr('x')),
				y = Number($(this).attr('y'));


				if($('.tile').hasClass('tile-invalid')){
					if(chancellor.board[x][y]==0 || !$(this).hasClass('tile-invalid')){
						return Materialize.toast("There is an invalid tile.", 2500);
					}
				}

				if(!chancellor.checkRookMove(chancellor.board, x,y) && chancellor.board[x][y]==0){
					$(this).addClass("tile-invalid");	
				}else if(!chancellor.checkKnightMove(chancellor.board, x,y) && chancellor.board[x][y]==0){
					$(this).addClass("tile-invalid");	
				}
				

				if($(this).hasClass("tile-invalid") && chancellor.board[x][y]==1){
					$(this).removeClass("tile-invalid");	
				}

				chancellor.board[x][y] = 
					chancellor.board[x][y] == 0 ? 1 : 0;


				if(chancellor.board[x][y] == 0){
					$(this).html("&nbsp;");
					chancellor.count--;
				}else if(chancellor.board[x][y] == 1){
					$(this).html("C");
					chancellor.count++;
				}

				if(chancellor.count > chancellor.board.length){
					$(this).click();
					return Materialize.toast("You reached the maximum chancellor count.", 2500);
				}

				if(chancellor.count == chancellor.board.length && !$('.tile').hasClass('tile-invalid')){
					swal("YOU WIN");
					$('#reset-btn').click();
				}

		});

	},

	checkRookMove : function(board, x, y){
		var arrayColumn = (arr, n) => arr.map(x => x[n]);

		if(~board[x].indexOf(1)) return false;
		if(~arrayColumn(board, y).indexOf(1)) return false;

		return true;
	},

	checkKnightMove : function(board, x, y){
		var dimension = board.length,
			i, j;
		
		if (dimension < 3) {
			return true;
		}

		for (i=1; i<=2; i++) {
			for(j=2; j>=1; j--) {
				if (i==j) continue;
				if (x+i < dimension) {
					if (y+j < dimension && board[x+i][y+j]) {
						return false;
					}
					if (y-j > -1 && board[x+i][y-j]) {
						return false;
					}
				}
				if (x-i > -1) {
					if (y+j < dimension && board[x-i][y+j]) {
						return false;
					}
					if (y-j > -1 && board[x-i][y-j]) {
						return false;
					}
				}
			}
		}

		return true;
	},

	createTable : function(element, dimensions){
		$(element).empty();

		chancellor.board = [];
		chancellor.count = 0;
		chancellor.win_flag = false;

		for(var i=0; i<dimensions; i++){
			chancellor.board[i] = [];
		
			for(var j=0; j<dimensions; j++){

				chancellor.board[i][j] = 0;

				var _class = (i+j)%2 ? "tile-white": "tile-black";
				$(element)
					.append([
						'<a class="tile ',_class,' waves-effect ',
						' waves-light btn" x=',i,' y=',j,' >',
						'&nbsp;</a>'
					].join(''));

			}
			$(element).append('<br/>');
		}
	},

	solveTable : function(){
		solver.init(chancellor);
	}

}