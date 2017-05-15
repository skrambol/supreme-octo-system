'use strict';

$(document).ready( function () {
	chancellor.init();
});

var chancellor = {
	board : [],

	solutions : [],

	count : 0,

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
				$('#chancellor-left').removeClass("s6");
				$('#chancellor-left').addClass("s12");
				$('#chancellor-right').hide();			
				chancellor.createTable('#board', chancellor.board.length);	
			});

		$('#solve-btn')
			.click(function(){
				$('#chancellor-left').removeClass("s12");
				$('#chancellor-left').addClass("s6");
				$('#chancellor-right').show();
				chancellor.solveTable();				
			});

		$('body').on('click', '#close-btn', function(){
			$('#chancellor-left').removeClass("s6");
			$('#chancellor-left').addClass("s12");
			$('#chancellor-right').hide();		
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
					$(this).html("&nbsp;");
					$(this).removeClass("tile-invalid");
					return Materialize.toast("You reached the maximum chancellor count.", 2500);
				}

				if(chancellor.count == chancellor.board.length && !$('.tile').hasClass('tile-invalid')){
					swal({
						title: "YOU WIN",
						text: "Do you want to clear the game board?",
						type: "success",
						showCancelButton: true,
						confirmButtonColor: "#DD6B55",
						confirmButtonText: "Yes, clear the board!",
						cancelButtonText: "No, I want to try another answer!",
						closeOnConfirm: false,
						closeOnCancel: false
					},
					function(isConfirm){
						if (isConfirm) {
							swal.close();
							$('#reset-btn').click();
						} else {
							swal.close();
						}
					});
				}

		});

	},

	checkRookMove : function(board, x, y){
		var arrayColumn = (arr, n) => arr.map(x => x[n]);
		var i;

		// if(~board[x].indexOf(1)) return false;
		// if(~arrayColumn(board, y).indexOf(1)) return false;
		for(i = 0; i < board.length; i++) {
			if(board[x][i] > 0 || board[i][y] > 0) {
				return false;
			}
		}

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

	createTable : function(element, dimensions, sol_flag){
		$(element).empty();

		chancellor.board = [];
		chancellor.count = 0;

		for(var i=0; i<dimensions; i++){
			chancellor.board[i] = [];
		
			for(var j=0; j<dimensions; j++){

				chancellor.board[i][j] = 0;

				var _class = (i+j)%2 ? "tile-white": "tile-black";
				var _id = (sol_flag) ? 'id="sol-'+i+'-'+j+'"' : '';
				var _click = (sol_flag) ? 'sol-tile ' : 'tile ';

				$(element)
					.append([
						'<a ',_id,' class="',_click, _class,' waves-effect ',
						' waves-light btn" x=',i,' y=',j,' >',
						'&nbsp;</a>'
					].join(''));

			}
			$(element).append('<br/>');
		}
	},

	createPagination : function(count, callback){
		$('#pagination').empty();
		$('#pagination').materializePagination({
			align: 'center',
			lastPage:  count,
			firstPage:  1,
			useUrlParameter: false,
			onClickCallback: function(page_no){
				if(callback) callback(page_no);
			}
		}); 
	},

	solveTable : function(){
		var board_copy = JSON.parse(JSON.stringify(chancellor.board)),
			count_copy = chancellor.count,
			solution = solver.init(chancellor);

		if(!solution.count){
			$('#solution-div').hide();
			$('#no-solution-div').show();
			chancellor.board = board_copy;
			chancellor.count = count_copy;	
			return;
		}else{
			$('#solution-div').show();
			$('#no-solution-div').hide();
		}

		$('#sol-count').empty();
		$('#sol-count').html(solution.count);

		chancellor.solutions = solution.boards;

		chancellor.createTable('#sol-board', chancellor.board.length, true);
		
		chancellor.createPagination(solution.count, function(index){
			var sol_board = chancellor.solutions[index-1],
				dimensions = sol_board.length;

			for(var i=0; i<dimensions; i++){
				for(var j=0; j<dimensions; j++){
					// console.log("i:"+i + " j:" +j + "--->" + sol_board[i][j]);
					var element = $('#sol-'+i+'-'+j),
						sol_value = sol_board[i][j];

					if(sol_value==0) element.html('&nbsp;');
					else if(sol_value==1) element.html('C');
					else if(sol_value==2){
						element.html('X');
					}
				}
			}
		});		

		chancellor.board = board_copy;
		chancellor.count = count_copy;		
	}

}