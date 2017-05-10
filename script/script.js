'use strict';

$(document).ready( function () {
	chancellor.init();
});

var chancellor = {
	board : [],

	init : function(){

		$('select').material_select();

		$('.dim-btn')
			.click(function(){
				chancellor.createTable('#board',$(this).attr("value"));
			});

		$('#dim-specify')
			.keypress(function (e) {
				if (e.keyCode == 13) {
					if(!Number($('#dim-specify').val())) return Materialize.toast("Please enter a valid dimension number.", 2500);
					chancellor.createTable('#board',Number($('#dim-specify').val()));
				}
			});

		$('body').on('click', '.tile', function(){
			var x = Number($(this).attr('x')),
				y = Number($(this).attr('y'));

				chancellor.board[x][y] = 
					chancellor.board[x][y] == 0 ? 1 : 0;


				if(chancellor.board[x][y] == 0) $(this).html("&nbsp;");
				else if(chancellor.board[x][y] == 1) $(this).html("C");

				console.table(chancellor.board);

		});

	},

	createTable : function(element, dimensions){
		$(element).empty();

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

		console.table(chancellor.board);
	}

}