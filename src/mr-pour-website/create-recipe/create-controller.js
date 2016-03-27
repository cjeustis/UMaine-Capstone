mrPour.controller('createController', function ($scope, $rootScope, $http, localStorageService, $location) {

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

    $scope.recipe = {};


    $scope.cancel = function() {
        $location.path('/dashboard/recipes');
    };

    $scope.saveRecipe = function(recipe) {
        recipe['table_name'] = $rootScope.userData.user_name;
        // Create sql table to hold user recipes
        $.ajax({
            type: 'post',
            url: 'php/saveRecipe.php',
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