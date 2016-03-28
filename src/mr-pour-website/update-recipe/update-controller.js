mrPour.controller('updateController', function ($scope, $rootScope, $http, localStorageService, $location) {

    $scope.modalShown = false;

    if (!$rootScope.recipeData) {
        $location.path('/dashboard/recipes');
    }

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

    $scope.recipe = $rootScope.recipeData;

    $scope.recipe.amount_1 = Number($scope.recipe.amount_1);
    $scope.recipe.amount_2 = Number($scope.recipe.amount_2);
    $scope.recipe.amount_3 = Number($scope.recipe.amount_3);
    $scope.recipe.amount_4 = Number($scope.recipe.amount_4);

    $scope.cancel = function() {
        $location.path('/dashboard/recipes');
        $scope.$apply();
    };

    $scope.saveRecipe = function(recipe) {
        recipe['table_name'] = $rootScope.userData.user_name;
        // Create sql table to hold user recipes
        $.ajax({
            type: 'post',
            url: 'php/updateRecipe.php',
            data: recipe,
            beforeSend: function() {
                $scope.modalShown = true;
            },
            success: function ( data ) {
                setInterval(function() {
                    $scope.modalShown = false;
                    $scope.$apply();
                }, 1000);
                $location.path('/dashboard/recipes');
                $scope.$apply();
            }
        });
    }
});