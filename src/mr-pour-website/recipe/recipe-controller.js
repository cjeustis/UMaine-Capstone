mrPour.controller('recipeController', function ($scope, $rootScope, $http, localStorageService, $location, $timeout) {
    $scope.get_recipes();

    $scope.modalShown = false;

    $scope.logout = function() {

        $.ajax({
            type : 'POST',
            url  : 'php/logout.php',
            beforeSend: function() {
                $scope.modalShown = true;
            },
            success :  function(response)
            {
                setInterval(function() {
                    $scope.modalShown = false;
                    doLogout();
                }, 1000);
            }
        });
    };

    function doLogout () {
        localStorageService.set('auth', false);
        localStorageService.set('expires', (Date.now() + 120000));
        $location.path('#/');
        $scope.$apply();
    }

    $('#updateRecipe').hide();
    $('#deleteRecipe').hide();
    $('#pourRecipe').hide();

    $scope._userRecipes = '';

    var gridDiv = document.querySelector('#myGrid');

    var columnDefs = [
        { headerName: 'ID', field: 'id', width: 50, editable: false, suppressSizeToFit: true },
        { headerName: 'Recipe Name', field: 'recipe_name', width: 200, editable: false },
        { headerName: 'Ingredient 1', field: 'ingredient_1', width: 200, editable: false },
        { headerName: 'Amount 1', field: 'amount_1', width: 150, editable: false },
        { headerName: 'Ingredient 2', field: 'ingredient_2', width: 200, editable: false },
        { headerName: 'Amount 2', field: 'amount_2', width: 150, editable: false },
        { headerName: 'Ingredient 3', field: 'ingredient_3', width: 200, editable: false },
        { headerName: 'Amount 3', field: 'amount_3', width: 150, editable: false },
        { headerName: 'Ingredient 4', field: 'ingredient_4', width: 200, editable: false },
        { headerName: 'Amount 4', field: 'amount_4', width: 150, editable: false },
        { headerName: 'Glass Type', field: 'glass_type', editable: false }
    ];

    var gridOptions = {
        columnDefs: columnDefs,
        enableSorting: true,
        enableColResize: true,
        rowDeselection: true,
        rowSelection: 'single',
        onRowSelected: rowSelectedFunc,
        onRowDeselected: rowDeslectedFunc
    };

    new ag.grid.Grid(gridDiv, gridOptions);

    function rowSelectedFunc (event) {
        $('#updateRecipe').show();
        $('#deleteRecipe').show();
        $('#pourRecipe').show();
        $scope.rowData = event.node.data;
    }

    function rowDeslectedFunc (event) {
        $('#updateRecipe').hide();
        $('#deleteRecipe').hide();
        $('#pourRecipe').hide();
        $scope.rowData = event.node.data;
    }

    $scope.createNewRecipe = function() {
        $location.path('/dashboard/recipes/create');
    };

    $scope.updateRecipe = function() {
        $rootScope.recipeData = $scope.rowData;
        $location.path('/dashboard/recipes/update');
    };

    $scope.pourRecipe = function() {
        while($rootScope.isBusy);
        $rootScope.isBusy = true;

        $scope.rowData['control'] = 'p';
        $.ajax({
            type: 'post',
            url: 'php/sendIngredientAmounts.php',
            data: $scope.rowData,
            beforeSend: function () {
                $scope.modalShown = true;
            },
            success: function () {
                // Show pouring dialog until finished
                var totalAmount = parseFloat($scope.rowData['amount_1'])
                    + parseFloat($scope.rowData['amount_2'])
                    + parseFloat($scope.rowData['amount_3'])
                    + parseFloat($scope.rowData['amount_4']);
                var pouringTime = totalAmount * 23000;  // Guess/check with little buffer
                setInterval(function() {
                    $scope.modalShown = false;
                    $rootScope.isBusy = false;
                    $scope.$apply();
                }, pouringTime);
            }
        });
    };

    $scope.deleteRecipe = function() {
        $scope.rowData['table_name'] = $rootScope.userData.user_name;
        // Delete selected recipe from table
        $.ajax({
            type: 'post',
            url: 'php/deleteRecipe.php',
            data: $scope.rowData,
            beforeSend: function() {

            },
            success: function ( data ) {
                $timeout(location.reload(true), 1000);
            }
        });
    };

    $scope.get_recipes = function() {
        var data = {};
        data['table_name'] = $rootScope.userData.user_name;
        //$scope.rowData['table_name'] = $rootScope.userData.user_name;
        // Delete selected recipe from table
        $.ajax({
            type: 'post',
            url: 'php/getRecipes.php',
            data: data,
            success: function ( data ) {
                $scope.$apply(function() {
                    $scope.hasRecipes = true;
                    $scope._userRecipes = data;
                    gridOptions.api.setRowData(data);
                });
            }
        });
    }

    //$.getJSON('php/getRecipes.php', function(data) {
    //    $scope.$apply(function() {
    //        $scope.hasRecipes = true;
    //        $scope._userRecipes = data;
    //        gridOptions.api.setRowData(data);
    //    });
    //}).error(function() { console.log('No recipes exist!'); $scope.hasRecipes = false; });
});