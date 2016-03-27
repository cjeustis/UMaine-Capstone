mrPour.controller('mainController', function ($scope, $rootScope, $location, $anchorScroll) {
    $scope.scrollTo = function(id) {
        $location.hash(id);
        $anchorScroll();
    };
});