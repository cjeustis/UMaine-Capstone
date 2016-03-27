mrPour.controller('loginController', function ($scope, $rootScope, $location, localStorageService) {

    if (localStorageService.get('auth')) {
        $location.path('/dashboard');
        $scope.$apply();
    }

    $("#login-form").validate({
        rules:
        {
            password: {
                required: true
            },
            user_email: {
                required: true,
                email: true
            }
        },
        messages:
        {
            password:{
                required: "please enter your password"
            },
            user_email: "please enter your email address"
        },
        submitHandler: submitForm
    });
    /* validation */

    /* login submit */
    function submitForm() {

        var enc = '';

        $("#password").keyup(function () {
            var value = $(this).val();
            enc = sha256_digest(value);
        }).keyup();

        $('#password').val(enc);

        var data = $("#login-form").serialize();

        $.ajax({

            type : 'POST',
            url  : 'php/login.php',
            data : data,
            beforeSend: function()
            {
                $("#error").fadeOut();
                $("#btn-login").html('<span class="glyphicon glyphicon-transfer"></span> &nbsp; sending ...');
            },
            success :  function(response)
            {
                if(response=="ok"){

                    $("#btn-login").html('<span class="glyphicon glyphicon-transfer"></span> &nbsp; Signing In ...');
                    setTimeout(angularRedirect(), 4000);
                }
                else{
                    $("#error").fadeIn(1000, function(){
                        $("#error").html('<div class="alert alert-danger"> <span class="glyphicon glyphicon-info-sign"></span> &nbsp; '+response+' !</div>');
                        $("#btn-login").html('<span class="glyphicon glyphicon-log-in"></span> &nbsp; Sign In');
                    });
                }
            }
        });
        return false;
    }

    function angularRedirect () {
        var now = Date.now();
        var expires = now + 3600000;
        //console.log('Authentication expires in: ' + ((expires - now)/60)/1000 + ' minutes');
        localStorageService.set('auth', true);
        localStorageService.set('expires', expires);
        $location.path('/dashboard');
        $scope.$apply();
    }

});